# double-Pendulum
It's one of my learning projects for the c language.

Feel free to use all my code, however you want to.

Credit:
-Raylib used for graphics
-For the physics eqations I used: 
  θ1'' = (−g (2 m1 + m2) sin θ1 − m2 g sin(θ1 − 2 θ2) − 2 sin(θ1 − θ2) m2 (θ2'2 L2 + θ1'2 L1 cos(θ1 − θ2))) / (L1 (2 m1 + m2 − m2 cos(2 θ1 − 2 θ2)))
  θ2'' = (2 sin(θ1 − θ2) (θ1'2 L1 (m1 + m2) + g(m1 + m2) cos θ1 + θ2'2 L2 m2 cos(θ1 − θ2))) / (L2 (2 m1 + m2 − m2 cos(2 θ1 − 2 θ2)))
  where θX'' is the xth pendulums angular accelaration for the current angels
  and θX' is the xth pendulums angular velocity for the current angles
  You can find theese equataions online. I found it here: https://www.myphysicslab.com/pendulum/double-pendulum-en.html
