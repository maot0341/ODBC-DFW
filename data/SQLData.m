function data = SQLData(db, sql)
% SQLData(db-connction, sql-statement)
rs = rset(db,sql)
i = 1;
tic
while 1,
	data(i) = fields (rs);
   if movenext(rs) < 1
      break
   end
   i = i + 1;
end
toc
close(rs);
