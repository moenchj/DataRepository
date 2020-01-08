#pragma once

#include <array>
#include <map>
#include <algorithm>
#include <string>
#include <exception>
#include <list>
#include <memory>

#include "PersistanceService.h"

template<typename T, size_t PageSize = 100, typename AllocatorT = std::allocator<T>>
class ResultSet
{
    public:
        class ResultSetIterator;    
        using value_type        = T;
        using reference         = value_type&;
        using const_reference   = const value_type&;
        using size_type         = std::size_t;
        using allocator_type    = AllocatorT;
        using pointer           = typename std::allocator_traits<allocator_type>::pointer;
        using const_pointer     = typename std::allocator_traits<allocator_type>::const_pointer;
        using iterator          = ResultSetIterator;

    private:
        using PageData = std::array<T, PageSize>;
        using Page = std::tuple<size_t, PageData>;
        using PageMapType = std::map<size_t, Page>;
        PageMapType pages;
        typename PageMapType::iterator test;
        PersistanceService<T>& persistanceService;
        std::string query;
        iterator endIterator;
        bool lastFetchFailed = false;

        class PagePosition
        {
            private:
                int page;
                size_type posInPage;
                bool isEndIter;
            
            public:
                PagePosition() : PagePosition(0, 0) {}

                PagePosition(const int page, const size_type posInPage) : page(page), posInPage(posInPage) {}

                PagePosition operator++(int)
                {
                    PagePosition tempPagePos(*this);
                    posInPage++;
                    if(posInPage >= PageSize)
                    {
                        posInPage = 0;
                        page++;
                    }
                    return tempPagePos;
                }

                bool operator==(const PagePosition& other) const
                {
                    return page == other.page && posInPage == other.posInPage;
                }

                size_type getPosInPage() const
                {
                    return posInPage;
                }

                int getPage() const
                {
                    return page;
                }

                PagePosition& set(const int page, const size_type posInPage)
                {
                    this->page = page;
                    this->posInPage = posInPage;
                    return *this;
                }
        };

    public:
        class ResultSetIterator
        {
            private:
                ResultSet* pResultSet;
                std::shared_ptr<PagePosition> position;

                ResultSetIterator(ResultSet<T, PageSize, AllocatorT>* pResultSet, const PagePosition& position )
                {
                    this->pResultSet = pResultSet;
                    this->position = std::make_shared<PagePosition>(position);
                }

                // check if iterator is on the position behind the last entry
                bool behindLastEntry()
                {
                    bool result = true;
                    typename PageMapType::iterator currentPageIter = pResultSet->pages.find(position->getPage());
                    bool pageFound = currentPageIter != pResultSet->pages.end();
                    if(pageFound)
                    {
                        bool lastPage = pResultSet->pages.rbegin()->first == position->getPage();
                        bool lastEntryInPage = std::get<0>(currentPageIter->second) == position->getPosInPage();
                        result = lastPage && lastEntryInPage;
                    }
                    return result;
                }

                bool hasMoreData()
                {
                    bool result = false;
                    typename PageMapType::iterator currentPageIter = pResultSet->pages.find(position->getPage());
                    bool pageFound = currentPageIter != pResultSet->pages.end();
                    if(pageFound)
                    {
                        size_t entriesInPage = std::get<0>(currentPageIter->second);
                        result = entriesInPage == PageSize;
                    }
                    else if (!pResultSet->lastFetchFailed)
                    {
                        result = true;
                    }
                    
                    return result;
                }

            public:
                // Return a reference to the value the iterator points to
                reference operator*() const
                {
                    int pageNumber = position->getPage();
                    size_type posInPage = position->getPosInPage();
                    typename PageMapType::iterator pageMapIter = pResultSet->pages.find(pageNumber);
                    if(pageMapIter != pResultSet->pages.end())
                    {
                        Page& page = pageMapIter->second;
                        size_type sizeOfPage = std::get<0>(page);
                        PageData& pageData =  std::get<1>(page);
                        if(posInPage < sizeOfPage)
                        {
                            return pageData[posInPage];
                        }
                    }
                    throw std::out_of_range("Position not in result set");
                }

                // Next value with pre increment (++iter)
                typename ResultSet::iterator& operator++() 
                {
                    if(*this !=  pResultSet->end())
                    {
                        (*position)++;
                        if(behindLastEntry() && hasMoreData())
                        {
                            pResultSet->fetchPage(position->getPage());
                        }
                    }
                    return *this;
                }

                // Next value with post increment (iter++)
                typename ResultSet::iterator operator++(const int) 
                {
                    iterator tempIterator = *this;
                    ++(*this);
                    return tempIterator;
                }

                // Make iterator point to the same position than the given iterator
                iterator& operator=(const typename ResultSet::iterator& other)
                {
                    *position = *(other->position);
                    return *this;
                }

                // Check if two iterators are pointing to the same element
                bool operator==(const typename ResultSet::iterator& other) const
                {
                    return *position == *(other.position);
                }

                // Check if two iterators are not pointing to the same element
                bool operator!=(const typename ResultSet::iterator& other) const
                {
                    return !(*this == other);
                }

                friend ResultSet<T, PageSize, AllocatorT>;
        };

    private:    
        // fetch the page with the given number from the persistance service
        bool fetchPage(const unsigned int pageNumber)
        {
            if(pages.find(pageNumber) == pages.end()) 
            {
                size_type start = pageNumber * PageSize;
                size_type end = start + PageSize;
                try{
                    auto queryResult = persistanceService.executeQuery(query, pageNumber * PageSize, PageSize);
                    if(queryResult.size() > 0)
                    {
                        std::array<T, PageSize> newPageData;            
                        std::transform(queryResult.begin(), queryResult.end(), newPageData.begin(), [](T row){ return row; });
                        pages.insert(std::pair<size_t, Page>(pageNumber, Page(queryResult.size(), newPageData)));
                        endIterator.position->set(pageNumber, queryResult.size() - 1)++;
                    }
                    else
                    {
                        lastFetchFailed = true;
                    }
                }
                catch(std::exception e)
                {
                    return false;
                }
            }
            return true;
        }

    public:            
        // Constructor
        ResultSet(const std::string& query, PersistanceService<T>& persistanceService) :
            query(query),
            persistanceService(persistanceService), 
            endIterator(this, PagePosition {0, 0})
        {
            this->fetchPage(0);
        }

        // get iterator to start
        iterator begin()
        {
            return typename ResultSet::iterator { this, PagePosition {0, 0} };
        }

        // get iterator to end
        const iterator& end()
        {
            return endIterator;
        }
};