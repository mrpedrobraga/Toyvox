package space3d.objects;

import java.awt.Color;
import java.awt.Graphics2D;
import static space3d.VMathUtils.*;
import java.awt.Image;
import java.awt.image.BufferedImage;
import space3d.Layout3D;
import space3d.Object3D;
import space3d.Vector;

public class Camera extends Object3D {

    public double gethROT() {
        return hROT;
    }

    public void sethROT(double hROT) {
        this.hROT = hROT;
    }

    public double getvROT() {
        return vROT;
    }

    public void setvROT(double vROT) {
        this.vROT = vROT;
    }

    public double gethFOV() {
        return hFOV;
    }

    public void sethFOV(double hFOV) {
        this.hFOV = hFOV;
    }

    public double getvFOV() {
        return vFOV;
    }

    public void setvFOV(double vFOV) {
        this.vFOV = vFOV;
    }

    /**
     * @param hROT the Horizontal Rotation of the camera
     * @param vROT the Vertical Rotation of the camera
     * @param hFOV the Horizontal F.O.V. of the camera
     * @param vFOV the Vertical F.O.V. of the camera
     */
    private double hROT, vROT, hFOV, vFOV;

    public BufferedImage render(Layout3D layout, int width, int height) {

        BufferedImage output = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = (Graphics2D) output.getGraphics();
        
        double rayAngleH, rayAngleV, x, y, a, b, px, py;

        //Does the raytracing stuff
        for (int ky = 0; ky < height; ky++) {
            for (int kx = 0; kx < width; kx++) {
                
                rayAngleH = angleLerp(gethROT() - gethFOV()/2, gethROT() + gethFOV()/2, getP(0, kx, width));
                rayAngleV = angleLerp(getvROT() - getvFOV()/2, getvROT() + getvFOV()/2, getP(0, ky, height));
                
                a = atan(rayAngleH);
                g.setColor(new Color((int) Math.floor(lerp(0, 255, (double)(kx)/width)), (int) Math.floor(lerp(0, 255, (double)(ky)/height)), 255));
                
                if(ky > 50 && ky < 250 && kx > 50 && kx < 250) g.drawLine(kx,ky,kx,ky);
            }
        }
        
        return output;
    }
    
    public Camera(Vector position, double hROT, double vROT, double hFOV, double vFOV){
        
        this.hROT = hROT; this.vROT = vROT;
        this.hFOV = hFOV; this.vFOV = vFOV;
        
        setPosition(position);
    }
}
