boost_regex_dep = meson.get_compiler('cpp').find_library('boost_regex')
avro_dep = meson.get_compiler('cpp').find_library('avrocpp')
thread_dep = dependency('threads')
jsoncpp_dep = dependency('jsoncpp')
protobuf_dep = dependency('protobuf')



srcs = ['main.cpp', 'proto/locations.pb.cc','Epoll/MessageQueue/MessageQueue.hh', 'Epoll/EpollConnection.cc',
           'Epoll/EpollIncoming.cc', 'Epoll/EpollInstance.cc', 'Epoll/EpollServer.cc',
           'Epoll/EpollEntry.cc', 'NetWorker.cc']

executable('server', srcs,
  dependencies: [thread_dep, boost_regex_dep, jsoncpp_dep, protobuf_dep]
)

