rm -f Database.sqlite
sqlite3 Database.sqlite "create table addresses (id integer primary key, street text, number text, zipcode text, city text)"
sqlite3 Database.sqlite "create table users (id integer primary key, firstname text, middlename text, surname text, birthday text, homeaddressid integer, FOREIGN KEY (homeaddressid) REFERENCES addresses(id))"

