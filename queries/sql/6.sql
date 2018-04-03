-- $ID$
-- TPC-H/TPC-R Forecasting Revenue Change Query (Q6)
-- Functional Query Definition
-- Approved February 1998
\timing on
select
	sum(l_extendedprice * l_discount) as revenue
from
	lineitem_1
where
	l_shipdate >= '1994-01-01'
	and l_shipdate < '1994-01-01' + interval '1' year
	and l_discount between 0.06 - 0.01 and 0.06 + 0.01
	and l_quantity < 24;
\timing off
