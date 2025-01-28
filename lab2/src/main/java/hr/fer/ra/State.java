package hr.fer.ra;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;

import static hr.fer.ra.Env.GRAV_CONSTANT;

public class State {
    private final List<Particle> particles;
    private final ArrayBlockingQueue<Particle> particleQueue;

    public State() {
        this(Env.MAX_POPULATION_SIZE);
    }

    public State(int maxCapacity) {
        List<Particle> temp = new ArrayList<>(maxCapacity);
        for (int i = 0; i < Env.MAX_POPULATION_SIZE; i++) {
            temp.add(ParticleFactory.createInactiveParticle());
        }
        this.particles = Collections.synchronizedList(temp);
        this.particleQueue = new ArrayBlockingQueue<>(maxCapacity);
    }

    private void addParticle(Particle particle) {
        boolean added = false;
        for (int i = 0; i < particles.size() && !added; i++) {
            if (particles.get(i).inactive()) {
                particles.set(i, particle);
                added = true;
            }
        }
        if (!added) {
            System.out.println("Too many particles! Particle not added");
        }
    }

    public void queueParticle(Particle particle) {
        particleQueue.add(particle);
    }

    public List<Particle> getParticles() {
        return particles;
    }

    public void populate(int count) {
        for (int i = 0; i < count; i++) {
            particleQueue.add(ParticleFactory.createRandomParticle());
        }
    }

    public void update(double deltaTime) {
        double[] fx = new double[particles.size()];
        double[] fy = new double[particles.size()];

        while (!particleQueue.isEmpty()) {
            addParticle(particleQueue.poll());
        }
        this.iterateOverActiveParticlePairs((i, j) -> {
            Particle particle1 = particles.get(i);
            Particle particle2 = particles.get(j);
            if (particle1.distance(particle2) < (particle1.getRadius() + particle2.getRadius())) {
                particles.set(i, particle1.combine(particle2));
                particle2.disable();
            }
        });
        this.iterateOverActiveParticlePairs((i, j) -> {
            Particle particle1 = particles.get(i);
            Particle particle2 = particles.get(j);
            double distance = particle1.distance(particle2);
            double dx = (particle1.getX() - particle2.getX()) / distance;
            double dy = (particle1.getY() - particle2.getY()) / distance;
            double force = GRAV_CONSTANT * particle1.getMass() * particle2.getMass() / Math.pow(distance, 2);
            if (Env.REVERSE_GRAVITY) {
                fx[i] += dx * force;
                fy[i] += dy * force;
                fx[j] += -dx * force;
                fy[j] += -dy * force;
            } else {
                fx[i] += -dx * force;
                fy[i] += -dy * force;
                fx[j] += dx * force;
                fy[j] += dy * force;
            }

        });

        for (int i = 0; i < particles.size(); i++) {

            if (!particles.get(i).inactive()) {
                particles.get(i).update(fx[i], fy[i], deltaTime);
            }
        }
    }

    private void iterateOverActiveParticlePairs(IntBiConsumer consumer) {
        int size = particles.size();
        for (int i = 0; i < size; i++) {
            if (particles.get(i).inactive()) {
                continue;
            }
            for (int j = i + 1; j < size; j++) {
                if (particles.get(j).inactive()) {
                    continue;
                }
                consumer.accept(i, j);
            }
        }
    }
}
