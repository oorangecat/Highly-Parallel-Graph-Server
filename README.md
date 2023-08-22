# Highly Parallel Graph Server

In the context of calculating multiple shortest-paths on a common graph, this server allows to a wide number of clients to:
- Connect in parallel to the server
- Send multiple Protobuf messages, both for contributing to the graph, adding walks and locations, and requesting a one-to-one or a one-to-all calculation
- Clear the graph

Multiple algorithmic callenges were addressed, such as merging of close nodes and synchronizing users' additions.

This is a solution for the Semestral Work of the Efficient Software course in CVUT, Prague. The Protobuf schema, aswell as the template of the Epoll library were given to the students as a template. 
