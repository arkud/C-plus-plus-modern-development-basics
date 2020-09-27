#include <utility>

#include "Common.h"

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class BasicShape : public IShape {
protected:
    shared_ptr<ITexture> texture;
    Point position = {0, 0};
    Size size = {0, 0};
public:
    void SetPosition(Point pos) override {
        position = pos;
    }

    Point GetPosition() const override {
        return position;
    }

    void SetSize(Size size_) override {
        size = size_;
    }

    Size GetSize() const override {
        return size;
    }

    void SetTexture(std::shared_ptr<ITexture> texture_) override {
        texture = texture_;
    }

    ITexture *GetTexture() const override {
        return texture.get();
    }
};

class Ellipse : public BasicShape {
public:
    Ellipse() {}

    Ellipse(const Point pos_, const Size s_, shared_ptr<ITexture> texture_) {
        position = pos_;
        size = s_;
        texture = move(texture_);
    }


    std::unique_ptr<IShape> Clone() const override {
        return unique_ptr<IShape>(new Ellipse(position, size, texture));
    }

    void Draw(Image &image) const override {
        Size t_size;
        if (texture) {
            t_size = texture->GetSize();
        }
        for (int y = position.y; y < position.y + size.height && y < image.size(); y++) {
            for (int x = position.x; x < position.x + size.width && x < image[y].size(); x++) {

                if (IsPointInEllipse({x - position.x, y - position.y}, size)) {
                    if (texture && x < position.x + t_size.width && y < position.y + t_size.height) {
                        image[y][x] = texture->GetImage()[y - position.y][x - position.x];
                    } else {
                        image[y][x] = '.';
                    }
                }
            }
        }
    }
};

class Rectangle : public BasicShape {
public:
    Rectangle() {}

    Rectangle(const Point pos_, const Size s_, shared_ptr<ITexture> texture_) {
        position = pos_;
        size = s_;
        texture = move(texture_);
    }


    std::unique_ptr<IShape> Clone() const override {
        return unique_ptr<IShape>(new Rectangle(position, size, texture));
    }

    void Draw(Image &image) const override {
        Size t_size;
        if (texture) {
            t_size = texture->GetSize();
        }
        for (int y = position.y; y < position.y + size.height && y < image.size(); y++) {
            for (int x = position.x; x < position.x + size.width && x < image[y].size(); x++) {

                if (texture && x < position.x + t_size.width && y < position.y + t_size.height) {
                    image[y][x] = texture->GetImage()[y - position.y][x - position.x];
                } else {
                    image[y][x] = '.';
                }
            }
        }
    }
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    if (shape_type == ShapeType::Rectangle) {
        return unique_ptr<IShape>(new Rectangle);
    } else if (shape_type == ShapeType::Ellipse) {
        return unique_ptr<IShape>(new Ellipse);
    }
}