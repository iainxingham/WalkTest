/*
 * File to create the test.sqlite database
 *
 * cat test1.sql | sqlite3 test1.sqlite
 *
 * Put test1.sqlite in `Database` directory of build folder
 */

CREATE TABLE `pats` (
        `id`			INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        `rxr`			TEXT UNIQUE,
        `nhs`			TEXT,
        `fname`			TEXT,
        `surname`		TEXT,
        `dob`			TEXT,
        `dateadded`		TEXT,
        `sex`			TEXT, 
        `updated`		TEXT
);

CREATE TABLE `physiology` (
		`id`			INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
		`pat_id`		INTEGER NOT NULL,
		`test_date`		TEXT,
		`measurement`	TEXT,
		`result`		FLOAT,
		
		FOREIGN KEY (pat_id) REFERENCES pats(id)
);

CREATE TABLE `grip` (
		`id`			INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
		`pat_id`		INTEGER NOT NULL,
		`test_date` 	TEXT,
		`dom_hand`		TEXT,
		`left_result`	FLOAT,
		`right_result`	FLOAT,
		
		FOREIGN KEY (pat_id) REFERENCES pats(id)
);

CREATE TABLE `walk_tests` (
		`id`			INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
		`short`			TEXT,
		`description`	TEXT,
		`start_dist`	INTEGER,	/* Initial distance in level (m)*/
		`dist_inc`		INTEGER,    /* Distance to increase by each level */
		`time_interval` INTEGER,	/* Length of each level (s)*/
		`levels`		INTEGER		/* Number of levels, 0 for until stopped by physiologist */
);

CREATE TABLE `iswt` (
		`id`					INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
		`pat_id`				INTEGER NOT NULL,
		`test_date`				TEXT,
		`test_type`				INTEGER NOT NULL,
		`start_borg_dyspnoea`	FLOAT,
		`start_borg_cr10`		FLOAT,
		`start_hr`				INTEGER,
		`start_o2`				INTEGER,
		`end_borg_dyspnoea`		FLOAT,
		`end_borg_cr10`			FLOAT,
		`end_hr`				INTEGER,
		`end_o2`				INTEGER,
		`min_o2`				INTEGER,
		`max_hr`				INTEGER,
		`duration`				INTEGER,
		`distance`				INTEGER,		
		
		FOREIGN KEY (pat_id) REFERENCES pats(id),
		FOREIGN KEY (test_type) REFERENCES walk_tests(id)
);

CREATE TABLE `shuttles` ( 
		`id`			INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
		`shuttle_no`	INTEGER,
		`level`			INTEGER,
		`start_time`	TEXT,
		`elapsed`		INTEGER,
		`pat_id`		INTEGER,
		`test_id`		INTEGER,
		
		FOREIGN KEY (pat_id) REFERENCES pats(id),
		FOREIGN KEY (test_id) REFERENCES iswt(id)		
);

INSERT INTO walk_tests (short, 
						description, 
						start_dist, 
						dist_inc, 
						time_interval, 
						levels) 
	   VALUES ("Standard ISWT", 
	           "Incremental shuttle walk test, starting at 0.5 metres per second",
	           30, 10, 60, 0);
INSERT INTO walk_tests (short, 
						description, 
						start_dist, 
						dist_inc, 
						time_interval, 
						levels) 
	   VALUES ("ISWT - frail", 
	           "Incremental shuttle walk test starting very slowly for frail patients. Limited to a maximum of 15 levels",
	           10, 10, 60, 15);
INSERT INTO walk_tests (short, 
						description, 
						start_dist, 
						dist_inc, 
						time_interval, 
						levels) 
	   VALUES ("ISWT - fast start", 
	           "Incremental shuttle walk test for younger or fitter patients, starting at 1 metre per second.",
	           60, 10, 60, 0);
