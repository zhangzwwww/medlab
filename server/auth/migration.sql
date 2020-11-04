CREATE table if not exists `user` (
		`id`       VARCHAR(255),
		`email`    VARCHAR(255),
		`ctime`    DATE,
		`role`     VARCHAR(40),
		`password` VARCHAR(255),
		PRIMARY KEY (`id`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE table if not exists `token` (
		`token`       VARCHAR(255),
		`id`          VARCHAR(255),
		`expired`     TINYINT(1),
		`expire_time` DATE,
		PRIMARY KEY (`token`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8
