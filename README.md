# Binary Search Tree Project

An implementation of the BST data structure. A `mainTree` BST holds several `itemTree`'s. BST's hold `NV_Node` containing a (N)ame and (V)alue. The structure can be queried. Query results are written to console and `out.txt`

In this project, `itemTree` contains `NV_Node`'s where the value is type `int`. BST's are sorted by name.


## Querying

All queries must be written in a `in.txt` file. Each query should be on it's own line. Sample `in.txt` and `out.txt` are given.

#### Initial Line and Building Trees

The first line of `in.txt` should be 3 integers `N` `I` `Q`. 
- `N` represents how many lines of `in.txt` are used to create and insert `itemTree`'s into `mainTree`. Each of the next `N` lines determine the name of the `itemTree`
- `I` represents how many lines of `in.txt` are used to insert nodes into the created `itemTree`'s. Each of the next `I` lines inserts one `NV_Node` into the specified `itemTree`
- `Q` represents how many lines of `in.txt` are used to query the created structure. Each of the next `Q` lines represents one query to the created trees

#### Available Queries

Queries are case sensitive
- `search`: search a specified `itemTree` for a `NV_Node` and display the result
- `count`: search for a specified `itemTree` and display the sum of all items within it
- `height_balance`: search for a specified `itemTree` and display the balance of it
- `item_before`: list the `NV_Node`'s within the specified `itemTree` that comes before the specified value
