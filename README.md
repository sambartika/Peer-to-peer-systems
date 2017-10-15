# Peer-to-peer-systems
# This project involves	designing and implementation of a mini distributed data storage and search system in the style of CHORD which implements a distributed hash tables (DHT) over a peer-to-peer network. Assume a collection of N nodes. A set of P values are to be distributed among the nodes. Here P >> N. Each value V has a unique key K. The key K controls which node will store the value V. The basic APIs that must be supported by the storage system are:
a.	put(V)
b.	V = get(K)
Note that there will be no centralized indexing service. All nodes must be functionally equivalent.
