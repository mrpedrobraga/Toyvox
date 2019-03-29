package test;

import javax.swing.JFrame;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.util.logging.Level;
import java.util.logging.Logger;
import voxel.VoxelModel;

public class PixelTester extends JFrame {

    VoxelModel object = new VoxelModel(new Color[100][100][100]);

    public PixelTester() {

        setTitle("Voxel to Pixel Test");
        setSize(640, 360);
        setResizable(false);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setVisible(true);

        run();
    }

    private final int fps = 60;
    private int ticks;
    private long dt;

    public void run() {
        
        int wx = 20, wy = 20, wz = 20;
        int max = 200;
        int min = 100;
        
        //init stuff
        for (int z = 0; z < wz; z++) {
            for (int y = 0; y < wy; y++) {
                for (int x = 0; x < wz; x++) {
                    if(Math.sqrt(Math.pow(Math.abs((double) x-wx/2),2) + Math.pow(Math.abs((double) y-wy/2),2) + Math.pow(Math.abs((double) z-wz/2),2) ) >= wx/2+1)
                        object.setVoxelAt(z, x, y, new Color(min + floor((max-min)*(double)x/wx),min + floor((max-min)*(double)y/wy),min + floor((max-min)*(double)z/wz)));
                }
            }
        }
        object.setVoxelAt(0, 0, 0, Color.blue);
        
        long time;
        while (true) {

            try {
                time = System.currentTimeMillis();

                update();
                draw();
                ticks++;
                time = System.currentTimeMillis() - time;
                dt = time;

                if (time > 0 && time < 1000 / fps) {
                    Thread.sleep(1000 / fps - time);
                }
            } catch (InterruptedException ex) {
                Logger.getLogger(PixelTester.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    public void update() {
        
        hRot += 1;
        vRot += 1;
    }

    public void draw() {
        BufferedImage img = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_4BYTE_ABGR);
        Graphics2D g = (Graphics2D) img.getGraphics();

        //Actually Drawing
        g.setColor(Color.white);
        g.fillRect(0, 0, getWidth(), getHeight());

        for (int z = 0; z < object.widthZ(); z++) {
            for (int y = 0; y < object.widthY(); y++) {
                for (int x = 0; x < object.widthX(); x++) {

                    if (object.getVoxelAt(x, y, z) != null) {
                        g.setColor(object.getVoxelAt(x, y, z));
                        g.drawLine(getWidth() / 2 + getX(x,y,z), getHeight() / 2 + getY(x,y,z), getWidth() / 2 + getX(x,y,z), getHeight() / 2 + getY(x,y,z));
                    }
                }
            }
        }

        int originX = getWidth()/2;
        int originY = getHeight()/2;
        
        //DRAWS TOYVOX LOGO AT THE CORNER
        g.setColor(Color.magenta);
        int xPoints[] = new int[]{originX, originX, originX - 20, originX - 20};
        int yPoints[] = new int[]{originY-10, originY + 20, originY+20, originY-10};
        g.fillPolygon(xPoints, yPoints, 4);
        
        //Draw buffer into canvas
        getGraphics().drawImage(img, 0, 0, this);
    }

    int hRot = 0, vRot = 60;
    double scale = 1;
    double dx = 10, dy = 10, dz = 10;
    
    public int getX(double x, double y, double z) {
        return floor( scale*(x-dx)*Math.cos(Math.toRadians(hRot)) + scale*(y-dy)*Math.sin(Math.toRadians(hRot)) );
    }
    
    public int getY(double x, double y, double z) {
        return floor( (scale*(x-dx)*Math.sin(Math.toRadians(hRot)) - scale*(y-dy)*Math.cos(Math.toRadians(hRot)))*Math.cos(Math.toRadians(vRot)) - scale*(z-dz)*Math.sin(Math.toRadians(vRot)) ); 
    }
    
    //USEFUL METHODS
    public void drawPixel(double x, double y, Graphics2D g, Color color) {

        int red = color.getRed();
        int green = color.getGreen();
        int blue = color.getBlue();
        double alpha = color.getAlpha();

        double lx = x - Math.floor(x);
        double ly = y - Math.floor(y);

        double a = (1 - lx) * (1 - ly);
        double b = lx * (1 - ly);
        double c = (1 - lx) * ly;
        double d = lx * ly;

        g.setColor(new Color(red, green, blue, floor(alpha * a)));
        g.drawLine(floor(x), floor(y), floor(x), floor(y));

        g.setColor(new Color(red, green, blue, floor(alpha * b)));
        g.drawLine(floor(x) + 1, floor(y), floor(x) + 1, floor(y));

        g.setColor(new Color(red, green, blue, floor(alpha * c)));
        g.drawLine(floor(x), floor(y) + 1, floor(x), floor(y) + 1);

        g.setColor(new Color(red, green, blue, floor(alpha * d)));
        g.drawLine(floor(x) + 1, floor(y) + 1, floor(x) + 1, floor(y) + 1);
    }

    public static int floor(double x) {
        return (int) Math.floor(x);
    }

    public static void main(String[] args) {

        new PixelTester();
    }
}
