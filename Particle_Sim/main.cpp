#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <random>

// CONST GLOBAL VALUES
const float GRAVITY = 0.89f;
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

std::random_device rd;
std::mt19937 gen(rd());

class Particle;
std::vector<Particle> particle_buffer;

class Particle {
private:
    sf::CircleShape shape;
    float radius;
    sf::Color color;
    float restitution = 0.9f;
    float friction = 0.9f;
    sf::Vector2f speed{ 20.f, 0.f };
    sf::Vector2i grid_position{ -1, -1 };
    static int next_id;
    int id;
public:
    Particle(float radius, float x, float y) {
        std::uniform_int_distribution<int> dist(0, 255);
        unsigned int r = dist(gen);
        unsigned int g = dist(gen);
        unsigned int b = dist(gen);
        color = sf::Color(r, g, b);

        shape = sf::CircleShape(radius);
        shape.setFillColor(color);
        this->radius = shape.getRadius();
        this->color = shape.getFillColor();
        shape.setOrigin({ radius, radius });
        shape.setPosition({x , y});
        this->id = next_id++;
    }

    float getRadius() {
        return radius;
    }
    sf::CircleShape& getShape() {
        return shape;
    }

    sf::Vector2f getSpeed() {
        return speed;
    }

    int getID() {
        return id;
    }

    void setSpeed(sf::Vector2f spd) {
        speed = spd;
    }

    void setGridPos(sf::Vector2i new_pos) {
        grid_position = new_pos;
    }

    void draw_Particle(sf::RenderWindow& window) {
        window.draw(shape);
    }

    void update() {
        speed.y += GRAVITY;
        shape.move(speed);

        if (shape.getPosition().y + radius >= WINDOW_HEIGHT) {
            shape.setPosition({ shape.getPosition().x, WINDOW_HEIGHT - radius});
            speed.y = -speed.y * restitution;
            speed.x *= friction;
        }

        if (shape.getPosition().y - radius <= 0) {
            shape.setPosition({ shape.getPosition().x, radius});
            speed.y = -speed.y * restitution;
        }

        if (shape.getPosition().x + radius >= WINDOW_WIDTH) {
            shape.setPosition({WINDOW_WIDTH - radius, shape.getPosition().y});
            speed.x = -speed.x * friction;
        }

        if (shape.getPosition().x - radius <= 0) {
            shape.setPosition({radius, shape.getPosition().y});
            speed.x = -speed.x * friction;
        }

    }

    void applySpeedColor() {
        float speed_magnitude = speed.x * speed.x + speed.y * speed.y;
        float normalized_speed = speed_magnitude / 625.f;

        if (normalized_speed > 1.0f) {
            normalized_speed = 1.0f;
        }
        else if (normalized_speed < 0.0f) {
            normalized_speed = 0.0f;
        }

        unsigned char r = (unsigned char)(255 * normalized_speed);
        unsigned char b = (unsigned char)(255 * (1.0f - normalized_speed));
        unsigned char g = 0;

        shape.setFillColor(sf::Color(r, g, b));
    }
    // ...



    void ScopeAdder(std::vector<Particle*>& tile_scope, std::vector<std::vector<std::vector<Particle*>>>& matrix, int x_parameter, int y_parameter) {
        int target_Y = grid_position.y + y_parameter;
        int target_X = grid_position.x + x_parameter;

        /*
        std::cout << "Position(px): (" << shape.getPosition().x << ", " << shape.getPosition().y << ") \n";
        std::cout << "Position(y, x): (" << grid_position.y << ", " << grid_position.x << ") \n";
        std::cout << "Acessando Grid (Y, X): (" << target_Y << ", " << target_X << ") \n\n";
        */

        for (int k = 0; k < matrix[target_Y][target_X].size(); k++) {
            tile_scope.push_back(matrix[target_Y][target_X][k]);
        }
    }

    std::vector<Particle*> getTileScope(std::vector<std::vector<std::vector<Particle*>>>& grid) {
        std::vector<Particle*> tile_scope;
        bool free_up = (grid_position.y - 1 >= 0) ? true : false;
        bool free_down = (grid_position.y + 1 < grid.size()) ? true : false;
        bool free_left = (grid_position.x - 1 >= 0) ? true : false;
        bool free_right = (grid_position.x + 1 < grid[0].size()) ? true : false;
        
        ScopeAdder(tile_scope, grid, 0, 0); 

        if (free_up) { ScopeAdder(tile_scope, grid, 0, -1); }

        if (free_down) { ScopeAdder(tile_scope, grid, 0, 1); }

        if (free_left) { ScopeAdder(tile_scope, grid, -1, 0); }

        if (free_right) { ScopeAdder(tile_scope, grid, 1, 0); }

        if (free_left  && free_up) { ScopeAdder(tile_scope, grid, -1, -1); }

        if (free_right && free_up) { ScopeAdder(tile_scope, grid, 1, -1); }

        if (free_left  && free_down) { ScopeAdder(tile_scope, grid, -1, 1); }

        if (free_right && free_down) { ScopeAdder(tile_scope, grid, 1, 1); }

        return tile_scope;
    }

    void collisionResolve(std::vector<std::vector<std::vector<Particle*>>>& grid) {
        if (grid_position.x != -1 && grid_position.y != -1) {
            for (Particle* particle_ptr : getTileScope(grid)) {
                if (particle_ptr->getID() > this->getID()) {

                    sf::CircleShape& circle1 = this->getShape();
                    sf::CircleShape& circle2 = particle_ptr->getShape();

                    sf::Vector2f distance_vector({circle1.getPosition().x - circle2.getPosition().x, circle1.getPosition().y - circle2.getPosition().y });
                    float distance_squared = distance_vector.x * distance_vector.x + distance_vector.y * distance_vector.y;
                    float radius_sum = circle1.getRadius() + circle2.getRadius();

                    if (distance_squared <= radius_sum * radius_sum) {

                        float hipotenuse = std::sqrt(distance_squared);
                        float intersection = radius_sum - hipotenuse;
                        if (!hipotenuse) { hipotenuse = 0.00001f; }

                        sf::Vector2f normalized_vector({distance_vector.x / hipotenuse, distance_vector.y / hipotenuse});
                        sf::Vector2f tangent_vector({ -normalized_vector.y, normalized_vector.x });

                        circle1.move({ normalized_vector.x * intersection / 2, normalized_vector.y * intersection / 2 });
                        circle2.move({ -normalized_vector.x * intersection / 2, -normalized_vector.y * intersection / 2 });
                    
                        float normal1_scalar = speed.x * normalized_vector.x + speed.y * normalized_vector.y;
                        float tangent1_scalar = speed.x * tangent_vector.x + speed.y * tangent_vector.y;
                        float normal2_scalar = particle_ptr->getSpeed().x * normalized_vector.x + particle_ptr->getSpeed().y * normalized_vector.y;
                        float tangent2_scalar = particle_ptr->getSpeed().x * tangent_vector.x + particle_ptr->getSpeed().y * tangent_vector.y;

                        float final_normal1 = normal2_scalar;
                        float final_tangent1 = tangent1_scalar * friction;
                        float final_normal2 = normal1_scalar;
                        float final_tangent2 = tangent2_scalar * friction;

                        sf::Vector2f normal_result1({ normalized_vector.x * final_normal1, normalized_vector.y * final_normal1 });
                        sf::Vector2f tangent_result1({ tangent_vector.x * final_tangent1, tangent_vector.y * final_tangent1});
                        sf::Vector2f normal_result2({ normalized_vector.x * final_normal2, normalized_vector.y * final_normal2});
                        sf::Vector2f tangent_result2({ tangent_vector.x * final_tangent2, tangent_vector.y * final_tangent2 });
                        
                        sf::Vector2f final_speed1 = normal_result1 + tangent_result1;
                        sf::Vector2f final_speed2 = normal_result2 + tangent_result2;

                        this->setSpeed(final_speed1);
                        particle_ptr->setSpeed(final_speed2);
                    }
                }
            }
        }
    }

};

int Particle::next_id = 0;

class Grid {
private:
    std::vector<std::vector<std::vector<Particle*>>> grid;
    int rows;
    int columns;
    sf::Vector2f tile_size{0.f, 0.f};
public:
    void fill_Grid() {
        for (int i = 0; i < rows; i++) {
            grid.push_back({});
            for (int j = 0; j < columns; j++) {
                grid[i].push_back({});
            }
        }
    }

    void buildGrid(float tile_size) {
        rows = (int)(WINDOW_HEIGHT / tile_size);
        columns = (int)(WINDOW_WIDTH / tile_size);
    }

    void InitGrid() {
        float max_tile_size = -1;
        for (int i = 0; i < particle_buffer.size(); i++) {
            Particle& particle = particle_buffer[i];
            max_tile_size = (particle.getRadius() > max_tile_size) ? particle.getRadius() : max_tile_size;
        }

        std::cout << max_tile_size;
        buildGrid(max_tile_size * 2);

        tile_size.x = (float)(WINDOW_WIDTH / columns);
        tile_size.y = (float)(WINDOW_HEIGHT / rows);

    }

    Grid() {
        InitGrid();
        fill_Grid();
    }
    
    void update() {
        grid.clear();
        fill_Grid();
        for (int i = 0; i < particle_buffer.size(); i++) {
            Particle& particle = particle_buffer[i];
            particle.update();
            sf::Vector2f pos = particle.getShape().getPosition();
            int grid_row = (int)pos.y / tile_size.y;
            int grid_col = (int)pos.x / tile_size.x;
            particle.setGridPos({ grid_col, grid_row });

            if (grid_row >= 0 && grid_row < rows && grid_col >= 0 && grid_col < columns) {
                grid[grid_row][grid_col].push_back(&particle);
            }
        }
    }


    std::vector<std::vector<std::vector<Particle*>>>& getGrid() {
        return grid;
    }

    int getRows() {
        return rows;
    }
    int getColumns() {
        return columns;
    }

    void printGridInfo() {
        system("cls");

        std::cout << "--- Grid Info ---\n";
        std::cout << "Rows (Y-axis divisions): " << rows << "\n";
        std::cout << "Columns (X-axis divisions): " << columns << "\n";
        std::cout << "Tile Size: (" << tile_size.x << ", " << tile_size.y << ")\n";
        std::cout << "Total cells: " << rows * columns << "\n";
        std::cout << "--- End of Grid Info ---\n";
    }

    void printGridContent() {

        std::cout << "--- Grid Content ---\n";
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                std::cout << grid[y][x].size() << " ";
            }
            std::cout << "\n";
        }
        std::cout << "--- End of Grid Content ---\n";
    }
};

const sf::Time reset_time = sf::seconds(0.05f);

int main() {
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "SFML no notebook!");
    window.setFramerateLimit(60);
    sf::Clock clock;

    particle_buffer.emplace_back(5.f, 0.f, 0.f);

    // Grid Init
    Grid grid;
    grid.printGridInfo();

    while (window.isOpen()) {

        if (clock.getElapsedTime() >= reset_time) {
            particle_buffer.emplace_back(5.f, 10.f, 10.f);
            clock.restart();
        }

        // Draw Event
        window.clear();
        grid.update();

        for (Particle& particle : particle_buffer) {
            particle.collisionResolve(grid.getGrid());
            particle.applySpeedColor();
            particle.draw_Particle(window);
        }

        window.display();

        // Close Window
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                system("cls");
                std::cout << "Bolas: " << particle_buffer.size();
                window.close();
            }
        }
    }
}