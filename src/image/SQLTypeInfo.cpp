//---------------------------------------------------------------------------
#if 0
	column ("TYPE_NAME",          SQL_CHAR,            128);
	column ("DATA_TYPE",          SQL_SMALLINT,          5);
	column ("COLUMN_SIZE",        SQL_INTEGER,          10);
	column ("LITERAL_PREFIX",     SQL_CHAR,            128);
	column ("LITERAL_SUFFIX",     SQL_CHAR,            128);
	column ("CREATE_PARAMS",      SQL_CHAR,            128);
	column ("NULLABLE",           SQL_SMALLINT,          5);
	column ("CASE_SENSITIVE",     SQL_SMALLINT,          5);
	column ("SEARCHABLE",         SQL_SMALLINT,          5);
	column ("UNSIGNED_ATTRIBUTE", SQL_CHAR,            128);
	column ("AUTO_UNIQUE_VALUE",  SQL_SMALLINT,          5);
	column ("LOCAL_TYPE_NAME",    SQL_CHAR,            128);
	column ("MINIMUM_SCALE",      SQL_SMALLINT,          5);
	column ("MAXIMUM_SCALE",      SQL_SMALLINT,          5);
	column ("SQL_DATA_TYPE",      SQL_SMALLINT,          5);
	column ("SQL_DATETIME_SUB",   SQL_SMALLINT,          5);
	column ("NUM_PREC_RADIX",     SQL_INTEGER,          10);
	column ("INTERVAL_PRECISION", SQL_SMALLINT,          5);

	////    1                    2                      3
	record ("CHAR",              SQL_CHAR,            128, "\"", "\"", TRUE, TRUE, 16);
	record ("INTEGER",           SQL_INTEGER,          10,   "",   "", TRUE, TRUE, 16);
	record ("FLOAT",             SQL_FLOAT,            13,   "",   "", TRUE, TRUE, 16);
	record ("DATETIME",          SQL_DATETIME,         16,  "#",  "#", TRUE, TRUE, 16);
	record ("TIMESTAMP",         SQL_TYPE_TIMESTAMP,   19,  "#",  "#", TRUE, TRUE, 16);
	record ("BIT",               SQL_BIT,               1,   "",   "", TRUE, TRUE, 16);
	record ("GUID",              SQL_GUID,             10,   "",   "", TRUE, TRUE, 16);
/*
	record ("NUMERIC",           SQL_NUMERIC,          16,   "",   "", TRUE, TRUE, 16);
	record ("DECIMAL",           SQL_DECIMAL,          16,   "",   "", TRUE, TRUE, 16);
	record ("SAMLLINT",          SQL_SMALLINT,         16,   "",   "", TRUE, TRUE, 16);
	record ("REAL",              SQL_REAL,             16,   "",   "", TRUE, TRUE, 16);
	record ("DOUBLE",            SQL_DOUBLE,           16,   "",   "", TRUE, TRUE, 16);
	record ("VARCHAR",           SQL_VARCHAR,          16,   "",   "", TRUE, TRUE, 16);
	record ("LONGVARCHAR",       SQL_LONGVARCHAR,      16,   "",   "", TRUE, TRUE, 16);
	record ("DATE",              SQL_TYPE_DATE,        16,  "#",  "#", TRUE, TRUE, 16);
	record ("TIME",              SQL_TYPE_TIME,        16,  "#",  "#", TRUE, TRUE, 16);
	record ("TINYINT",           SQL_TINYINT,          16,   "",   "", TRUE, TRUE, 16);
	record ("BIGINT",            SQL_BIGINT,           16,   "",   "", TRUE, TRUE, 16);
	record ("BINARY",            SQL_BINARY,           16,   "",   "", TRUE, TRUE, 16);
	record ("VARBINARY",         SQL_VARBINARY,        16,   "",   "", TRUE, TRUE, 16);
	record ("LONGVARBINARY",     SQL_LONGVARBINARY,    16,   "",   "", TRUE, TRUE, 16);
	record ("DATE",              SQL_TYPE_DATE ,       16,  "#",  "#", TRUE, TRUE, 16);
	record ("TIME",              SQL_TYPE_TIME ,       16,  "#",  "#", TRUE, TRUE, 16);
	record ("TIMESTAMP",         SQL_TYPE_TIMESTAMP ,  16,  "#",  "#", TRUE, TRUE, 16);
*/
};
#endif