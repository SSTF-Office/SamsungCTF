create table packets (idx int(10) primary key auto_increment, send int(10) unsigned, recv int(10) unsigned, msg text, index recv_idx(recv));
create table consumes (idx int(10) unsigned primary key);

# Add some hints
INSERT INTO `packets` VALUES (1,1,0,'zEwd4Qfh6165yF8ZjP/K1RcIuUTSMO1F2R9mVSfLoFepaQYvLQklJYopwz2H3ABC'),(2,1,0,'zEwd4Qfh6165yF8ZjP/K1RcIuUTSMO1F2R9mVSfLoFepaQYvLQklJYopwz2H3ABC'),(3,1,3463874446,'C4YX8YwK1IOLeVusr7uZBju1W6KFCpSkx5xGRL6msdQ4KDuNHuC94sSO66O8VsW9'),(4,1,0,'zEwd4Qfh6165yF8ZjP/K1RcIuUTSMO1F2R9mVSfLoFepaQYvLQklJYopwz2H3ABC'),(5,1,3463874446,'C4YX8YwK1IOLeVusr7uZBju1W6KFCpSkx5xGRL6msdQ4KDuNHuC94sSO66O8VsW9'),(10000,1,0,'zEwd4Qfh6165yF8ZjP/K1RcIuUTSMO1F2R9mVSfLoFepaQYvLQklJYopwz2H3ABC'),(10001,1,3463874446,'C4YX8YwK1IOLeVusr7uZBju1W6KFCpSkx5xGRL6msdQ4KDuNHuC94sSO66O8VsW9');
insert into `consumes` values (0), (1), (3463874446);
