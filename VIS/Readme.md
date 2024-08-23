# 3D Modeling Visualization Tool

Welcome to the 3D Modeling Visualization Tool! This is a visual tool designed for script-based modeling. You can input a modeling script and send it via network messages. The tool will parse the script and render a 3D model based on your instructions.

## Features

### Basic Shapes

You can create basic shapes using simple scripts. Here are a few examples:

- **Draw a Box**:
  ```plaintext
  {md:box 1 1 1}setup();draw();
  ```

- **Draw a Sphere**:
  ```plaintext
  {md:sphere 1}setup();draw();
  ```

- **Draw a Cylinder**:
  ```plaintext
  {md:cylinder 1 2}setup();draw();
  ```

### Combining Shapes

You can also combine different shapes in one script:

```plaintext
{a{md:box 1 1 1}b{md:sphere 1}}setup();draw();
```

### Surface Modeling

The tool supports surface modeling through coordinate interpolation. For example, you can create a mask effect with the following script:

```plaintext
{
    c0{x:0;y:1}
    c01{x:0;y:-2}
    
    c1{md:c;x:-2;rz:-45;ry:-45}
    c1_{md:c;x:-2;rz:45;ry:-45}
    c2{md:c;x:-1;rz:-45;ry:-45}
    c3{md:c;x:-0.5;rz:45;ry:-45}
    c2_{md:c;x:-1;rz:45;ry:-45}
    c3_{md:c;x:-0.5;rz:-45;ry:-45}
    c4{md:c;}
    
    d10{md:ccurve lerptan c1 c0;ts:28}
    d12{md:ccurve lerp c1 c2;ts:10}
    d23{md:ccurve lerptan c2 c3;ts:10}
    d24{md:ccurve lerptan c2_ c3_;ts:10}
    d34{md:ccurve lerp c3 c4;ts:10}
    d50{md:ccurve lerptan c1_ c01;ts:28}
    
    dd2{md:combine d12 d23 d34}
    
    e{md:face lerp d10 dd2}
    ee{{e}{e;m:x}}
    dd3{md:combine d12 d24 d34}
    
    e1{md:face lerp dd3 d50}
    eee{{e1}{e1;m:x}}
}
setup();draw(ee,eee);
```

### Branching Effects

You can create complex branching effects as well:

```plaintext
{
    a1{x:-1;rz:-90}a2{md:C;x:0;rz:90}a3{x:1;rz:90}
    c1{md:ccurve lerptan a1 a2}c2{c1;x:1}

    up{md:ccurve lerptan a1 a3;ts:31}
    cc{md:combine c1 c2;z:2}
    upface{md:face lerp up cc}
    branch{{upface}{upface;m:y}}
}setup();draw(branch);
```

## Expandability

I will continue to add more shape scripts. The script format is designed to be intuitive and can be easily understood through the examples provided, so there is no need for an extensive introduction.

For a detailed introduction to the scripts, please visit: [PHG-Algebra Documentation](https://github.com/panguojun/PHG-Algebra).

## Conclusion

This project aims to create a tree-structured scripting language capable of describing complex 3D structures. It is complete, powerful, and unique. I hope you enjoy using it!
