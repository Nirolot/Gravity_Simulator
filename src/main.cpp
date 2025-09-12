#include "config.h"

// Funzione per calcolare la velocità orbitale
float calculateOrbitalVelocity(float centralMass, float distance) {
    // v = sqrt(GM/r)
    float scaledDistance = distance * scaling_factor;
    return sqrt(G * centralMass / scaledDistance);
}

class Object {
public:
    std::vector<float> position;
    std::vector<float> previousPosition;
    std::vector<float> velocity;
    std::vector<float> acceleration;
    float radius;
    int res = 50;
    float mass;
    float deltaTime = simulationSpeed;
    bool shouldDelete = false;
    bool firstFrame = true;

    Object(std::vector<float> position, std::vector<float> velocity, float radius, float mass) {
        this->position = position;
        this->velocity = velocity;
        this->radius = radius;
        this->mass = mass;
        this->acceleration = { 0.0f, 0.0f };

        // Inizializza previousPosition per il primo frame usando Euler
        this->previousPosition = {
            position[0] - velocity[0] * deltaTime,
            position[1] - velocity[1] * deltaTime
        };
    }

    void addAcceleration(float x, float y) {
        acceleration[0] += x;
        acceleration[1] += y;
    }

    void updatePosVerlet() {
        // Verlet Integration: x(t+dt) = 2*x(t) - x(t-dt) + a(t)*dt²
        float newPosX = 2.0f * position[0] - previousPosition[0] + acceleration[0] * deltaTime * deltaTime;
        float newPosY = 2.0f * position[1] - previousPosition[1] + acceleration[1] * deltaTime * deltaTime;

        // Salva la posizione corrente come "precedente"
        previousPosition[0] = position[0];
        previousPosition[1] = position[1];

        // Aggiorna alla nuova posizione
        position[0] = newPosX;
        position[1] = newPosY;

        // Calcola velocità per riferimento: v = (x(t) - x(t-dt)) / dt
        velocity[0] = (position[0] - previousPosition[0]) / deltaTime;
        velocity[1] = (position[1] - previousPosition[1]) / deltaTime;
    }

    void DrawCircle() {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(position[0], position[1]);

        for (int i = 0; i <= res; ++i) {
            float angle = 2.0f * M_PI * i / res;
            float x = position[0] + cos(angle) * radius;
            float y = position[1] + sin(angle) * radius;
            glVertex2f(x, y);
        }

        glEnd();
    }

    void calculate_distance(float& dx, float& dy, float& distance, const Object& obj) {
        dx = obj.position[0] - this->position[0];
        dy = obj.position[1] - this->position[1];
        distance = hypot(dx, dy);
    }

    void checkCollisions(std::vector<Object>& objs) {
        for (auto& obj : objs) {
            if (&obj == this || obj.shouldDelete) continue;
            float dx, dy, distance;
            calculate_distance(dx, dy, distance, obj);
            if (distance < this->radius + obj.radius) {
                obj.shouldDelete = true;
                this->shouldDelete = true;
                return;
            }
        }
    }

    void calculate_pull_factor(std::vector<Object>& objs) {
        for (auto& obj : objs) {
            if (&obj == this || obj.shouldDelete) continue;
            float dx, dy, distance;
            calculate_distance(dx, dy, distance, obj);

            if (distance == 0) continue;

            std::vector<float> direction = { dx / distance, dy / distance };
            distance *= scaling_factor; // Scaling factor

            float Gforce = (G * obj.mass * this->mass) / (distance * distance);
            //std::cout << "G Force: " << Gforce << std::endl;
            float acc = Gforce / this->mass;
            //std::cout << "Acc x: " << acc * direction[0] << "\nAcc y: " << acc * direction[1] << std::endl;

            // Aggiungi accelerazione invece di modificare direttamente la velocità
            addAcceleration(acc * direction[0], acc * direction[1]);
        }
    }
};

GLFWwindow* StartGLFW();

int main() {
    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // Calcolo velocità orbitale
    float distance = (screenWidth / 2.0f - 200.0f) * scaling_factor;
    float v_orb = calculateOrbitalVelocity(7.35e22, distance);
    std::cout << v_orb << std::endl;

    // Creazione oggetti con velocità iniziale corretta
    std::vector<Object> objs = {
        Object({200.0f, screenHeight / 2.0f}, {0.0f, 50.0f}, 20.0f, 7.35e22), // Luna
        Object({screenWidth / 2.0f, screenHeight / 2.0f}, {0.0f, -50.0f}, 20.0f, 7.35e22) // Terra
    };

    float lastTime = glfwGetTime();

    while(!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Calcola forze gravitazionali e aggiorna posizioni
        for (auto& obj : objs) {
            if (!obj.shouldDelete) {
                obj.calculate_pull_factor(objs);
                obj.updatePosVerlet();
            }
        }

        // Controlla collisioni
        for (auto& obj : objs) {
            if (!obj.shouldDelete) {
                obj.checkCollisions(objs);
            }
        }

        // Disegna oggetti
        for (auto& obj : objs) {
            if (!obj.shouldDelete) {
                obj.DrawCircle();
            }
        }

        // Rimuovi oggetti marcati per la cancellazione
        objs.erase(std::remove_if(objs.begin(), objs.end(),
            [](const Object& obj) { return obj.shouldDelete; }),
            objs.end());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* StartGLFW() {
    if (!glfwInit()) {
        std::cout << "failed to initialize glfw, panic!" << std::endl;
        return nullptr;
    }
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "gravity_sim", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}