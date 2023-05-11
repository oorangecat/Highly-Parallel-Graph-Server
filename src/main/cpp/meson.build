avro_dep = meson.get_compiler('cpp').find_library('avrocpp')
thread_dep = dependency('threads')
jsoncpp_dep = dependency('jsoncpp')
protobuf_dep = dependency('protobuf')
boost_dep = dependency('boost')


srcs = ['main.cpp', 'proto/locations.pb.cc','Epoll/MessageQueue/MessageQueue.hh', 'Epoll/EpollConnection.cc',
           'Epoll/EpollIncoming.cc', 'Epoll/EpollInstance.cc', 'Epoll/EpollServer.cc',
           'Epoll/EpollEntry.cc', 'Graph/Graph.cc', 'Graph/Edge.cc', 'Graph/Node.cc', 'Graph/PointMap.cc', 'NetWorker.cc']

executable('server', srcs,
  dependencies: [thread_dep, boost_dep, jsoncpp_dep, protobuf_dep]
)

