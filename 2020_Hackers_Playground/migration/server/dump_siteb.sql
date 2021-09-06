create table member (idx int(10) primary key auto_increment, is_admin int(10), user_id varchar(12) unique, password varchar(32));
create table log (idx int(10), log text);
create table information (protocol varchar(10), host varchar(30), id varchar(100), pw varchar(100));
create table reset_history (no int(10) primary key auto_increment, val datetime);
insert into information values ('FTP', '127.0.0.1', 'flag', 'SCTF{M34n1ng1e55_a1r_g4p}');
insert into reset_history (val) values (now());
