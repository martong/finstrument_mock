#include <gmock/gmock.h>
#include <access_private.hpp>
#include <unordered_map>

class Turtle {
    int x = 0, y = 0;
public:
    void PenUp() { /* ... */ }
    void PenDown() { /* ... */ }
    void Forward(int distance) { /* ... */ }
    void Turn(int degrees) { /* ... */ }
    void GoTo(int x, int y) { /* ... */ }
    int GetX() const { return x; }
    int GetY() const { return y; }
};

class Painter {
    Turtle turtle;
public:
    void DrawLine(int x0, int y0, int x1, int y1) {
        turtle.GoTo(x0, y0);
        turtle.PenDown();
        turtle.GoTo(x1, y1);
        turtle.PenUp();
    }
    // ...
};

class MockTurtle {
public:
  MOCK_METHOD0(PenUp, void());
  MOCK_METHOD0(PenDown, void());
  MOCK_METHOD1(Forward, void(int distance));
  MOCK_METHOD1(Turn, void(int degrees));
  MOCK_METHOD2(GoTo, void(int x, int y));
  MOCK_CONST_METHOD0(GetX, int());
  MOCK_CONST_METHOD0(GetY, int());
};

MockTurtle& GetMockObject(Turtle*) {
    static MockTurtle m;
    return m;
}

namespace proxy {
    void GoTo(Turtle* self, int x, int y) {
        return GetMockObject(self).GoTo(x, y);
    }
    void PenDown(Turtle* self) {
        return GetMockObject(self).PenDown();
    }
    void PenUp(Turtle* self) {
        return GetMockObject(self).PenUp();
    }
    void Forward(Turtle* self, int distance) {
        return GetMockObject(self).Forward(distance);
    }
    void Turn(Turtle* self, int degrees) {
        return GetMockObject(self).Turn(degrees);
    }
    int GetX(Turtle* self) {
        return GetMockObject(self).GetX();
    }
    int GetY(Turtle* self) {
        return GetMockObject(self).GetY();
    }
}

ACCESS_PRIVATE_FIELD(Painter, Turtle, turtle)

TEST(TurtleTest, TestDrawLine) {
    using ::testing::AtLeast;

    Painter painter;
    Turtle& turtle = access_private::turtle(painter);
    MockTurtle& mockTurtle = GetMockObject(&turtle);

    EXPECT_CALL(mockTurtle, PenDown()).Times(AtLeast(1));
    painter.DrawLine(0, 0, 10, 10);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
