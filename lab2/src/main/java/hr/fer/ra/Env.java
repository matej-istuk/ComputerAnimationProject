package hr.fer.ra;

public interface Env {
    boolean REVERSE_GRAVITY = false;
    boolean BOUNCE = true;
    int POPULATE_SIZE = 30;
    int MAX_POPULATION_SIZE = 1000;
    // Between 1 and 0
    double SCALE = 0.1;
    double SPAWN_SCALE = 0.5;
    double INITIAL_SPEED_MULTIPLIER = 4;
    double GRAV_CONSTANT = 0.5;
    boolean AUTO_SPAWN = true;
    double SPAWN_DELAY = 2.;
}
