# Documentation

## UDBMS

### create table

```sql
create table <name> (
	<ident> <type> [constraint][,
	<ident> <type> [constraint],...
	]
);
```

### drop table
```sql
drop table <name>, [<name>, ...];
```

### show create table 
```sql
show create table <ident>;
```

### insert into table
```sql
insert into <table name> (<name>, [<name>, ...]) values (<value>, [<value>, ..]);
```

### select from table
```sql
select <column>, [<column>, ..] from <table name>;
```
