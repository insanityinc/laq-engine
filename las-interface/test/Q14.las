/*
 * Linear Algebra Script 14
 */

A = filter( match( p.type , "PROMO" ) )			//1 <-- #p
B = dot( A, l.partkey )							//1 <-- #l
C = filter( l.shipdate >= '' )					//1 <-- #l
D = filter( l.shipdate < '' )					//1 <-- #l
E = hadamard( B, C )							//1 <-- #l
F = hadamard( E, D )							//1 <-- #l
G = map( l.extendedprice*(1-l.discount) )		//1 <-- #l
H = hadamard( F, G )							//1 <-- #l
I = sum( G )									//1 <-- 1
J = sum( H )									//1 <-- 1
K = map( 100.00 * J / I )						//1 <-- 1
