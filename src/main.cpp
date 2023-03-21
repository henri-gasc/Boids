#include "Boids.hpp"
#include <SFML/Graphics.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

int main() {
    const int nbr_boids = 50;

    boost::ptr_vector<Boid> all_Boids(nbr_boids);
    WindowHandler app;
    RandomNumberGenerator rng(0, 200);

    for (int i = 0; i < nbr_boids; i++) {
        Boid *test = new Boid(i*10, i*10, rng.pick()/100, rng.pick()/100);
        all_Boids.push_back(test);
    }

    while (app.isRunning()) {
        for (int i = 0; i < nbr_boids; i++) {
            Boid boid = all_Boids[i];
            app.draw(boid.shape);
            boid.update_pos();
            all_Boids[i] = boid;
        }
        app.display();
    }

    return 0;
}
