package hr.fer.ra;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class ParticlePanel extends JPanel implements MouseListener, ActionListener {
    private final State state;
    private final static int FPS = 60;
    private final static int DELAY = 1000/FPS;
    private int savedX;
    private int savedY;
    private final Timer timer;

    public ParticlePanel(State state) {
        this.state = state;
        this.timer = new Timer(DELAY, this);
        this.timer.start();
        this.addMouseListener(this);
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Toolkit.getDefaultToolkit().sync();
        Graphics2D g2 = (Graphics2D)g;
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        for (Particle particle: state.getParticles()) {
            if (!particle.inactive()) {
                g2.setColor(particle.getColor());
                if (Env.BOUNCE) {
                    g2.fillOval(
                            (int) Math.round(getWidth() * (particle.getX() - particle.getRadius())),
                            (int) Math.round(getHeight() * (particle.getY() - particle.getRadius())),
                            (int) Math.round(getWidth() * 2 * particle.getRadius()),
                            (int) Math.round(getHeight() * 2 * particle.getRadius())
                    );
                } else {
                    for (int i = -1; i < 2; i++){
                        for (int j = -1; j < 2; j++){
                            g2.fillOval(
                                    (int) Math.round(getWidth() * (particle.getX() + i - particle.getRadius())),
                                    (int) Math.round(getHeight() * (particle.getY() + j - particle.getRadius())),
                                    (int) Math.round(getWidth() * 2 * particle.getRadius()),
                                    (int) Math.round(getHeight() * 2 * particle.getRadius())
                            );
                        }
                    }
                }

            }
        }
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        state.populate(Env.POPULATE_SIZE);
    }

    @Override
    public void mousePressed(MouseEvent e) {
        savedX = e.getX();
        savedY = e.getY();
    }

    @Override
    public void mouseReleased(MouseEvent e) {
        if (Math.sqrt(Math.pow(((double) savedX - e.getX()) / this.getWidth(), 2) + Math.pow(((double) savedY - e.getY()) / this.getHeight(), 2)) >= 0.05) {
            state.queueParticle(ParticleFactory.createParticle(
                    ((double) e.getX()) / this.getWidth(),
                    ((double) e.getY()) / this.getHeight(),
                    ((double) this.savedX - e.getX()) / this.getWidth(),
                    ((double) this.savedY - e.getY()) / this.getHeight()

            ));
        }
    }

    @Override
    public void mouseEntered(MouseEvent e) {

    }

    @Override
    public void mouseExited(MouseEvent e) {

    }

    @Override
    public void actionPerformed(ActionEvent e) {
        repaint();
    }
}
