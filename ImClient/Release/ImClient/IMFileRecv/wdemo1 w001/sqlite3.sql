select * from tb_msg where id='1234' order by msgid desc;

drop table tb_friends;

 create table tb_groupinfo (
 id  INTEGER primary key autoincrement,
 name  TEXT  not null,
 dscb   TEXT         
 )
 
 drop table groupinfo ;
 
 
 insert into tb_friends values ('124', 'dd', 'ff', 2);
 
 insert into tb_groupinfo values (null,'Ä°ÉúÈË','ff');
 
 
 drop table tb_groupinfo;
 
 
 CREATE TABLE tb_friends (	
id VARCHAR(20) NOT NULL,
nickname VARCHAR(20),
datatime VARCHAR(30) NOT NULL,	
groupname   VARCHAR(30)  not null,
CONSTRAINT  pk primary key (id)
 );
 
 drop table tb_friends;
 select * from tb_friends;
 

 delete  from tb_msg where msgid="2";
 
 insert into tb_friends values ('12345', 'just_bg',datetime('now'), 'dsds');
 
 insert into tb_friends values ('123''45', 'just_bg',datetime('now'), 'dsds');
 
 select * from tb_friends;
 
 
 
 
 insert into tb_friends values ('12345', 'just_bg',datetime('now'), 'dsds');
 
 
 
 
 
 
 
 
 
 update tb_friends set nickname ='hjello'  ,datatime=datetime('2014-07-09 11:30:57') where  id='12345';
 select * from tb_friends;
 
 
 select * from tb_msg order by msgid desc  limit 5;
 delete  from tb_msg  where msgid in ( select msgid from tb_msg order by msgid desc limit 2);
 select * from tb_msg where msgid in (select msgid from 
	tb_msg  where (id = '1234' and  msgid>=8) order by msgid asc limit 1);
	
	
delete from tb_msg;









CREATE TABLE tb_msg(id varchar(20) not null,
		      msgid INTEGER primary key autoincrement,
		      fromorto smallint not null,
		      msgcontent varchar(500),
		      msgres   varchar(30) ,
		      datetime varchar(30), 
		      )


drop table tb_msg;

CREATE TABLE tb_msg(id varchar(20) not null,
		      msgid INTEGER primary key autoincrement,
		      fromorto smallint not null,
		      msgcontent varchar(500),
		      msgres   varchar(30) ,
		      datetime varchar(30), 
		      fontname  varchar(20),
		      fontsize  smallint ,
		      fontbold      smallint,
		      fontitalics   smallint, 
		      fontunderline  smallint,
		      fontcolor      INTEGER);










select * from tb_msg;



select * from (select * from tb_msg  where id='8669959' order by msgid  asc) limit 5 offset 10;
select * from tb_msg  where id='8669959'

select * from tb_msg  where id='8669959' order by msgid  desc

select * from tb_msg  where id='8669959' order by msgid  asc limit 5 offset 10

select* from tb_msg where id='wdemo1:w148' and datetime < datetime('now') order by datetime desc limit 10 offset 0


select * from tb_msg where id='wdemo1:w148' and datetime < datetime('2014-11-30 11:26:13','localtime') order by datetime  desc limit 51 offset 0

select * from tb_msg where id='wdemo1:w148' and  msgid < 1000 order by  msgid  desc limit 51 offset 0


select * from tb_msg where id='wdemo1:w148' and datetime < datetime('2014-12-03 24:00:00') 
and datetime >datetime('2014-12-03 00:00:00')  order by msgid asc limit 51 offset 0

select count(*) from tb_msg where id='wdemo1:w148' and datetime < datetime('2014-11-30 24:00:00') order by msgid asc limit 1

select * from tb_msg where id='wdemo1:w148' and datetime < datetime('2014-11-30 11:26:13')

create table tb_loginname (id, name, tm);
				 


select time from tb_localuser where id='11223344'
	
	
select * from tb_loginname  where tm between datetime('2014-02-12 20:35:10.30') and datetime('2014-11-12 20:35:30.30');
	
 create table tb_loginid (id, loginid, tm);

 insert into tb_loginid (id, tm) values ('ww' , datetime('now'));




delete  from tb_localuser


/////  tb_friend

insert into tb_localuser (id, nickname, logintime, lastlogintime, res, type, sex, loginid) values ('111111111', 'haibo', datetime('now'),0, '1', '1', 'Male', 'Jimimii');


insert into tb_loginid values ('111111111', 'hiwoow', datetime('now'));

select * from tb_loginid , tb_localuser where tb_loginid.id = tb_localuser.id;


select  tb_localuser.id, tb_loginid.loginid  from tb_loginid , tb_localuser  where tb_loginid.id = tb_localuser.id;


select count(*) from tb_localuser where  id='111111111' ;

// update tb_friends set nickname ='hjello'  ,datatime=datetime('2014-07-09 11:30:57') where  id='12345';
update tb_localuser set nickname='Jim' , res='ssss' , lastlogintime= datetime('now') where id ='111111111'

update tb_localuser set nickname='jimi', logintime='0', lastlogintime=datetime('now'), res='sssssss', type='', sex='Male', loginid='Just_bg' where id='111111111'


select * from tb_localuser ;

CREATE TABLE if not exists tb_localuser(id varchar(20) not null,nickname varchar(20),logintime int not null, lastlogintime varchar(20) not null,res		varchar(20) not null  default '',type varchar(20) not null, sex  varchar(20) not null, loginid varchar(20) not null, primary key(id));						  insert into tb_localuser values ('wdemo1:w019','Îâº£Ñó','0','', '','', '', wdemo1:w019);						  insert into  tb_localconfig (id) values ('wdemo1:w019');						  insert into tb_loginid values ('wdemo1:w019', 'wdemo1:w019',  datetime('now')) ;	
a
drop table tb_localconfig

select * from tb_localuser; 


drop table tb_groups

create table tb_friends (type,memsrc,id,  nickname,  
			sex,picurl, signature, tempmember,
			mobile, telephone,address,email,init,post,
			department,constraint pk primary key (id) );
create table tb_groups (id, name, system default 1, primary key (id, name));
 
insert into tb_groups  values ('www', '111', '1');


select * from tb_friends;


select * from tb_groups


