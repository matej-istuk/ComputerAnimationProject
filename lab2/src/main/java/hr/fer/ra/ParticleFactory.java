package hr.fer.ra;

import java.util.concurrent.ThreadLocalRandom;

public class ParticleFactory {
    public static Particle createRandomParticle() {
        double radius = ThreadLocalRandom.current().nextDouble() / 20 * Env.SCALE;
        return new Particle(
                ThreadLocalRandom.current().nextDouble(radius + (1.-Env.SPAWN_SCALE) / 2., 1-radius - (1.-Env.SPAWN_SCALE) / 2),
                ThreadLocalRandom.current().nextDouble(radius + (1.-Env.SPAWN_SCALE) / 2., 1-radius - (1.-Env.SPAWN_SCALE) / 2),
                (ThreadLocalRandom.current().nextDouble() - 0.5) * Env.SCALE * Env.INITIAL_SPEED_MULTIPLIER,
                (ThreadLocalRandom.current().nextDouble() - 0.5) * Env.SCALE * Env.INITIAL_SPEED_MULTIPLIER,
                ThreadLocalRandom.current().nextDouble() * 100,
                radius
            );
    }

    public static Particle createInactiveParticle() {
        return new Particle(
                -1,
                -1,
                -1,
                -1,
                -1,
                1
        );
    }

    public static Particle createParticle(double x, double y, double vx, double vy) {
        double radius = ThreadLocalRandom.current().nextDouble() / 20 * Env.SCALE;
        return new Particle(
                Math.min(Math.max(radius, x), 1-radius),
                Math.min(Math.max(radius, y), 1-radius),
                vx,
                vy,
                ThreadLocalRandom.current().nextDouble() * 100,
                radius
        );
    }

}
