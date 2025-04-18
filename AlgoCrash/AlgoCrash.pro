QT       += core gui widgets
CONFIG   += c++17

TEMPLATE = app
TARGET = AlgoCrash

# Include paths
INCLUDEPATH += include
INCLUDEPATH += third_party

# App sources
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    third_party/Box2D/Collision/b2BroadPhase.cpp \
    third_party/Box2D/Collision/b2CollideCircle.cpp \
    third_party/Box2D/Collision/b2CollideEdge.cpp \
    third_party/Box2D/Collision/b2CollidePolygon.cpp \
    third_party/Box2D/Collision/b2Collision.cpp \
    third_party/Box2D/Collision/b2Distance.cpp \
    third_party/Box2D/Collision/b2DynamicTree.cpp \
    third_party/Box2D/Collision/b2TimeOfImpact.cpp \
    third_party/Box2D/Collision/Shapes/b2ChainShape.cpp \
    third_party/Box2D/Collision/Shapes/b2CircleShape.cpp \
    third_party/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    third_party/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    third_party/Box2D/Common/b2BlockAllocator.cpp \
    third_party/Box2D/Common/b2Draw.cpp \
    third_party/Box2D/Common/b2Math.cpp \
    third_party/Box2D/Common/b2Settings.cpp \
    third_party/Box2D/Common/b2StackAllocator.cpp \
    third_party/Box2D/Common/b2Timer.cpp \
    third_party/Box2D/Dynamics/b2Body.cpp \
    third_party/Box2D/Dynamics/b2ContactManager.cpp \
    third_party/Box2D/Dynamics/b2Fixture.cpp \
    third_party/Box2D/Dynamics/b2Island.cpp \
    third_party/Box2D/Dynamics/b2World.cpp \
    third_party/Box2D/Dynamics/b2WorldCallbacks.cpp \
    third_party/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    third_party/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    third_party/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    third_party/Box2D/Dynamics/Contacts/b2Contact.cpp \
    third_party/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    third_party/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    third_party/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    third_party/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    third_party/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    third_party/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2Joint.cpp \
    third_party/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    third_party/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    third_party/Box2D/Rope/b2Rope.cpp

HEADERS += \
    include/mainwindow.h \
    $$files(third_party/Box2D/**/*.h, true)

# UI Forms
FORMS += ui/mainwindow.ui
message("Box2D CPP files: $$files(third_party/Box2D/**/*.cpp, true)")
