# Tree-of-Space
You are given a complete, balanced N-Ary Tree and must support Q queries. There are 3 kinds of queries. Return true or false depending on whether the query was successful.

Lock(v, id) - Lock vertex v for user - id
Unlock(v, id) - If vertex v is locked by the same id, unlock it.
Upgrade(v, id) - If v is unlocked and has at least one locked vertex in it's subtree and every locked vertex in the subtree of v is locked by id, unlock them and lock v instead.

Further, here are some additional constraints >> 
   1. A vertex cannot be locked if it has any locked ancestors or descendants, by any ID.
   2. When a vertex is upgraded, it's locked descendants are automatically unlocked.
   3. An upgrade operation is not possible if the vertex is already locked or has any locked ancestors
   4. An unlock operation is only possible if the vertex is already locked and locked by the same id


	 * INPUT
	 * n = total number of nodes
	 * m = number of child per node
	 * q = number of queries
	 *
	 * next 'n' lines = node name in string
	 * next 'q' lines = queries with (opcode, string, uid)
	 *
	 * opcode => 1 = Lock, 2 = Unlock, 3 = Upgrade
	


Example Input:
7 -> n ( number of nodes )
2 -> m ( number of links to a node)
4 -> q ( number of queries )
World
Asia
Africa
India
China
SouthAfrica
Egypt
1 China 9
1 India 9
3 Asia 9
2 India 9
