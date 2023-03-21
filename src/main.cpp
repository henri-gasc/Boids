#include "Boids.hpp"
#include <SFML/Graphics.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

int main() {
    const int nbr_boids = 50;

    boost::ptr_vector<Boid> all_Boids(nbr_boids);
    WindowHandler app;
    RandomNumberGenerator rng(-100, 200);

    for (int i = 0; i < nbr_boids; i++) {
        Boid *test = new Boid(WINDOW_HEIGHT/2, WINDOW_WIDTH/2, &rng);
        all_Boids.push_back(test);
    }

    while (app.isRunning()) {
        for (int i = 0; i < nbr_boids; i++) {
            Boid boid = all_Boids[i];
            boid.apply_flock(all_Boids);
            boid.update_pos();
            app.draw(boid.shape);
            all_Boids[i] = boid;
        }
        app.display();
    }

    return 0;
}
