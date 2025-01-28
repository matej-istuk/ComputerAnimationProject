package hr.fer.ra;

import java.awt.*;
import java.util.concurrent.ThreadLocalRandom;

public class Particle {
    private double x;
    private double y;
    private double vx;
    private double vy;
    private double age;
    private double maxAge;
    private double radius;
    private Color color;
    private double massCache;

    public Particle(double x, double y, double vx, double vy, double maxAge, double radius, Color color) {
        this(x, y, vx, vy, 0, maxAge, radius, color);
    }

    public Particle(double x, double y, double vx, double vy, double maxAge, double radius) {
        this(x, y, vx, vy, maxAge, radius, new Color(
                ThreadLocalRandom.current().nextInt(255),
                ThreadLocalRandom.current().nextInt(255),
                ThreadLocalRandom.current().nextInt(255)
        ));
    }

    public Particle(double x, double y, double vx, double vy, double age, double maxAge, double radius, Color color) {
        this.x = x;
        this.y = y;
        this.vx = vx;
        this.vy = vy;
        this.age = age;
        this.maxAge = maxAge;
        this.radius = radius;
        this.massCache = -1;
        this.color = color;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public double getVx() {
        return vx;
    }

    public double getVy() {
        return vy;
    }

    public double getAge() {
        return age;
    }

    public double getMaxAge() {
        return maxAge;
    }

    public double getRadius() {
        return radius * (maxAge - age) / maxAge;
    }

    public double getMass() {
        if (massCache == -1) {
            massCache = Math.pow(getRadius(), 2) * Math.PI;
        }
        return massCache;
    }

    public Color getColor() {
        return color;
    }

    public void updateVelocity(double fx, double fy, double deltaTime) {
        double ax = fx/getMass();
        double ay = fy/getMass();
        this.vx += ax * deltaTime;
        this.vy += ay * deltaTime;
    }

    public void update(double fx, double fy, double deltaTime) {
        updateVelocity(fx, fy, deltaTime);
        updatePosition(deltaTime);
        updateAge(deltaTime);
        massCache = -1;
    }

    public void updatePosition(double deltaTime) {
        this.x += vx * deltaTime;
        this.y += vy * deltaTime;
        if (Env.BOUNCE) {
            if (this.x < getRadius() || this.x > 1 - getRadius()) {
                this.vx *= -1;
            }
            if (this.y < getRadius() || this.y > 1 - getRadius()) {
                this.vy *= -1;
            }
        } else {
            if (this.x < 0 || this.x > 1) {
                this.x = 1 - this.x;
            }
            if (this.y < 0 || this.y > 1) {
                this.y = 1 - this.y;
            }
        }
    }

    public void updateAge(double deltaTime) {
        this.age += deltaTime * getMass() * 1000;
    }

    public boolean inactive() {
        return age >= maxAge;
    }

    public double distance(Particle other) {
        if (Env.BOUNCE) {
            return Math.sqrt(Math.pow(this.x - other.x, 2) + Math.pow(this.y - other.y, 2));
        } else {
            double distance = 100000;

            for (int i = -1; i < 2; i++){
                for (int j = -1; j < 2; j++) {
                    distance = Math.min(distance, Math.sqrt(Math.pow(this.x + i - other.x, 2) + Math.pow(this.y + j - other.y, 2)));
                }
            }
            return distance;
        }
    }

    public void disable() {
        this.age = maxAge + 1;
    }

    public Particle combine(Particle other) {
        double massSum = this.getMass() + other.getMass();
        double particle1Influence = this.getMass() / massSum;
        double particle2Influence = other.getMass() / massSum;
        Color color = new Color(
                (int) (this.color.getRed() * particle1Influence + other.color.getRed() * particle2Influence),
                (int) (this.color.getGreen() * particle1Influence + other.color.getGreen() * particle2Influence),
                (int) (this.color.getBlue() * particle1Influence + other.color.getBlue() * particle2Influence)
                );
        return new Particle(
                this.getX() * particle1Influence + other.getX() * particle2Influence,
                this.getY() * particle1Influence + other.getY() * particle2Influence,
                this.getVx() * particle1Influence + other.getVx() * particle2Influence,
                this.getVy() * particle1Influence + other.getVy() * particle2Influence,
                (this.getMaxAge() + other.getMaxAge()) / 2.,
                Math.sqrt(Math.pow(this.getRadius(), 2) + Math.pow(other.getRadius(), 2)),
                color
        );
    }
}
