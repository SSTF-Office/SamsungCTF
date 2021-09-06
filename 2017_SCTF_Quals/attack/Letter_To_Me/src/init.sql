use ltom;
drop table if exists users;
drop table if exists files;
drop table if exists notes;
drop table if exists FLAG;

CREATE TABLE users (
    username varchar(256),
    password varchar(256),
    PRIMARY KEY(username)
);

INSERT INTO users values ("admin", "as21njlqw021kbj21iohds9usfdmnasd09iu");

CREATE TABLE files (
   id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
   realname varchar(256),
   path varchar(256)
);

CREATE TABLE notes (
   username varchar(256),
   data varchar(5000)
);

CREATE TABLE LTOM_fl3g (
  flag varchar(256)
);

INSERT INTO FLAG values ("SCTF{Enj0y_y0ur_0nly_life}");
