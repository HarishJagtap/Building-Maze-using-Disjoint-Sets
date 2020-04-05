# Disjoint set forest with union by rank and path compression

# For union by rank, we need to maintain a rank for each node,
# which is an upper bound on the height of the node. We
# make the root with smaller rank point to the root with larger 
# rank during a UNION operation.

# path compression, we use it during FIND-SET operations to make
# each node on the find path point directly to the root. Path 
# compression does not change any ranks.

# So we maintain two lists, parent and rank list.
# Parent list ==> parent[node x] = index to parent node of node x
# Rank list ==> rank[node x] = upper bound on the height of the node x
# single node in a tree will have rank of zero

class DisjointSet: 

    # create 'n' disjoint sets with one node in each set
    def __init__(self, n):

        self.size = n
        # for union by rank, initially rank[node] = 0
        self.rank = [0] * n

        # initially, each node points to itself, parent[node x] = node x
        self.parent = [i for i in range(n)]
  
    # Find the representative(root) node of the set that node x belongs to
    def find(self, x): 

        if (x >= self.size):
            return

        # The representative(root) node points to itself
        # If Not the representative(root) node, then
        if (self.parent[x] != x):
              
            # Recursively move through the tree to the root node
            # Path compression by pointing each node directly to the root
            self.parent[x] = self.find(self.parent[x]) 

        return self.parent[x] 

    # The UNION operation has two cases, depending on whether the roots of 
    # the trees have equal rank. If the roots have unequal rank, we make the 
    # root with higher rank the parent of the root with lower rank, but the
    # ranks themselves remain unchanged. If, instead, the roots have equal ranks, 
    # we arbitrarily choose one of the roots as the parent and increment its rank
    def union(self, x, y): 

        if (x >= self.size) or (y >= self.size):
            return  
        
        # Find representatives(roots) of sets that node x and y belong to
        xroot = self.find(x) 
        yroot = self.find(y) 
  
        # If node x and node y are already in same set
        if xroot == yroot: 
            return
  
        # make the root with higher rank the parent of the root with lower rank
        if self.rank[xroot] < self.rank[yroot]: 
            self.parent[xroot] = yroot
        elif self.rank[xroot] > self.rank[yroot]: 
            self.parent[yroot] = xroot 
        else:
            # the roots have equal ranks, we arbitrarily choose one of 
            # the roots as the parent and increment its rank
            self.parent[yroot] = xroot 
            self.rank[xroot] += 1

    # Return the no of disjoint sets
    # no of disjoint sets = no of representatives(roots) = no of nodes with (parent[node] = node)
    def no_of_sets(self):

        count = 0

        for i in range(self.size):
            if self.parent[i] == i:
                count += 1

        return count