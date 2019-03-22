package space3d.objects;

import java.awt.Color;
import space3d.Vector;

public class VoxelModel {
    
    private byte[][][][] voxels = new byte[4][][][];
    
    //Returns the RED of a voxel
    public byte getRed(Vector position){
        return voxels[0][(int) position.getZ()][(int) position.getY()][(int) position.getX()];
    }
    //Returns the GREEN of a voxel
    public byte getGreen(Vector position){
        return voxels[1][(int) position.getZ()][(int) position.getY()][(int) position.getX()];
    }
    
    //Returns the BLUE of a voxel
    public byte getBlue(Vector position){
        return voxels[2][(int) position.getZ()][(int) position.getY()][(int) position.getX()];
    }
    
    //Returns the TRANSPARENCY of a voxel
    public byte getAlpha(Vector position){
        return voxels[3][(int) position.getZ()][(int) position.getY()][(int) position.getX()];
    }
    
    //Returns the Color of a voxel
    public Color getColor(Vector position){
        return new Color(getRed(position),getGreen(position),getBlue(position),getAlpha(position));
    }
    
    //Sets a single voxel
    public void setVoxel(Vector position, Color color){
        voxels[0][(int) position.getZ()][(int) position.getY()][(int) position.getX()] = (byte) color.getRed();
        voxels[1][(int) position.getZ()][(int) position.getY()][(int) position.getX()] = (byte) color.getGreen();
        voxels[2][(int) position.getZ()][(int) position.getY()][(int) position.getX()] = (byte) color.getBlue();
        voxels[3][(int) position.getZ()][(int) position.getY()][(int) position.getX()] = (byte) color.getAlpha();
    }
}