class_<std::vector<AbsCoord> >("AbsCoordVec")
   .def(vector_indexing_suite<std::vector<AbsCoord> >());

class_<std::vector<RRCoord> >("RRCoordVec")
   .def(vector_indexing_suite<std::vector<RRCoord> >());

class_<std::vector<RobotVisionInfo> >("RobotVisionInfoVec")
   .def(vector_indexing_suite<std::vector<RobotVisionInfo> >());

// class_<std::vector<RefereeHandsVisionInfo> >("RefereeHandsVisionInfoVec")
//    .def(vector_indexing_suite<std::vector<RefereeHandsVisionInfo> >());

class_<std::vector<RobotObstacle> >("RobotObstacleVec")
   .def(vector_indexing_suite<std::vector<RobotObstacle> >());

class_<std::vector<BallInfo> >("BallInfoVec")
   .def(vector_indexing_suite<std::vector<BallInfo> >());

class_<std::vector<float> >("FloatVec")
   .def(vector_indexing_suite<std::vector<float> >());

class_<std::vector<int> >("IntVec")
   .def(vector_indexing_suite<std::vector<int> >());

class_<std::vector<std::vector<int> > >("Int2DVec")
   .def(vector_indexing_suite<std::vector<std::vector<int> > >());

class_<std::vector<bool> >("BoolVec")
   .def(vector_indexing_suite<std::vector<bool> >());
