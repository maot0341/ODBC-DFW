function data = Test()
% ODBC-Test
db = dbase('ODBC Test','','')
rs = rset(db,'SELECT rid,val,ztr FROM P_FPRGSG tab WHERE ztr IS NOT NULL ORDER BY time,val')
i = 0;
rid = 0;
tic
while 1,
	record = fields (rs);
%	i = record.rid + 1;
	j = record.val + 1;
   if rid ~= record.rid,
      rid = record.rid;
      i = i + 1;
   end 
   v = record.ztr;
   i
   j
   v
	data(i,j) = v;
   if movenext(rs) < 1
      break
   end
end
toc
close(rs);
close(db);
