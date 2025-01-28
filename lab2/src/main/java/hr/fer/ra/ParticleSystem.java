package hr.fer.ra;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.Set;

public class ParticleSystem extends JFrame {
    private final static int WIDTH = 950;
    private final static int HEIGHT = 950;
    private boolean closed = false;
    private final Random rand = new Random();

    private final State state;

    public ParticleSystem() {
        setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                close();
            }
        });
        setLocation(0, 0);
        setSize(WIDTH, HEIGHT);
        setTitle("JParticleSystem");
        this.state = new State();
        add(new ParticlePanel(state));

        SwingWorker<Object, Object> swingWorker = new SwingWorker<>() {
            private static double NANO_IN_SECOND = 1_000_000_000.;
            @Override
            protected Object doInBackground() throws Exception {
                long newTime;
                long oldTime = System.nanoTime();
                long lastSpawnTime = -1;
                while (!closed) {
                    newTime = System.nanoTime();
                    if (Env.AUTO_SPAWN && (newTime - lastSpawnTime) / NANO_IN_SECOND > Env.SPAWN_DELAY) {
                        state.populate(Env.POPULATE_SIZE);
                        lastSpawnTime = newTime;
                    }
                    state.update((newTime - oldTime) / NANO_IN_SECOND);
                    oldTime = newTime;
                }
                return null;
            }
        };

        swingWorker.execute();
    }

    private void close() {
        closed = true;
        System.exit(0);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new ParticleSystem().setVisible(true));
    }

}