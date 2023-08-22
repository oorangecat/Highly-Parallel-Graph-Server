# Highly Parallel Graph Server

In the context of calculating multiple shortest-paths on a common graph, this server allows to a wide number of clients to:
- Connect in paralle to the server
- Send multiple Protobuf messages, both for contributing to the graph, adding walks and locations, and requesting a one-to-one or a one-to-all calculation
- Clear the graph

This is a solution for the Semestral Work of the Efficient Software course in CVUT, Prague
