thread_dep = dependency('threads')
jsoncpp_dep = dependency('jsoncpp')
protobuf_dep = dependency('protobuf')
boost_dep = dependency('boost')
urcu_dep = dependency('liburcu-qsbr', static: true)


srcs = ['main.cpp', 'proto/locations.pb.cc','Epoll/MessageQueue/MessageQueue.hh', 'Epoll/EpollConnection.cc',
           'Epoll/EpollIncoming.cc', 'Epoll/EpollInstance.cc', 'Epoll/EpollServer.cc', 'Epoll/EpollGraphMessage.cc',
           'Epoll/EpollEntry.cc', 'Epoll/EpollResult.cc', 'Graph/Graph.cc', 'Graph/QuadTree.cc', 'Graph/Edge.cc', 'Graph/Node.cc', 'Graph/PointMap.cc',
           'Epoll/Connection.hh', 'Graph/Message.hh', 'NetWorker.cc', 'GraphWorker.cc']

executable('server', srcs, cpp_args: '-DUSE_RCU',
  dependencies: [thread_dep, boost_dep, jsoncpp_dep, protobuf_dep, urcu_dep]
)

