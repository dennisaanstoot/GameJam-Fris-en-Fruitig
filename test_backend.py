# program template for Spaceship
import simplegui
import math
import random

# globals for user interface
WIDTH = 800
HEIGHT = 600

#max number of rocks, can make the game harder or simpler
MAX_ROCKS = 12

#angle velocity tweak for quicker or slower controlls
ANGLE_VEL = 0.03*math.pi

START_SCREEN 	= 0
GAME_STARTED 	= 1
GAME_OVER 		= 2

class ImageInfo:
    def __init__(self, center, size, radius = 0, lifespan = None, animated = False):
        self.center = center
        self.size = size
        self.radius = radius
        if lifespan:
            self.lifespan = lifespan
        else:
            self.lifespan = float('inf')
        self.animated = animated

    def get_center(self):
        return self.center

    def get_size(self):
        return self.size

    def get_radius(self):
        return self.radius

    def get_lifespan(self):
        return self.lifespan

    def get_animated(self):
        return self.animated

# helper functions to handle transformations
def angle_to_vector(ang):
    return [math.cos(ang), math.sin(ang)]

def dist(p,q):
    return math.sqrt((p[0] - q[0]) ** 2+(p[1] - q[1]) ** 2)


# Ship class
class Ship:
    def __init__(self, pos, vel, angle, image, info):
        # missile image - shot1.png, shot2.png, shot3.png
        self.missile_info = ImageInfo([5,5], [10, 10], 3, 40)
        self.missile_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/shot2.png")

        self.missile_sound = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/missile.mp3")
        self.missile_sound.set_volume(.5)
        self.explosion_sound = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/explosion.mp3")

        self.pos = [pos[0],pos[1]]
        self.vel = [vel[0],vel[1]]
        self.thrust = False
        self.angle = angle
        self.angle_vel = 0
        self.accel = 0.09
        self.friction = 0.992
        self.image = image
        self.image_center = info.get_center()
        self.image_size = info.get_size()
        self.radius = info.get_radius()
        self.ship_thrust_sound = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/thrust.mp3")
        self.missiles = set()
        self.missile_mult = 10
        
    def get_position(self):
        return self.pos
    
    def get_radius(self):
        return self.radius
        
    def rotate_left(self):
        self.angle_vel = -ANGLE_VEL
        
    def rotate_right(self):
        self.angle_vel = ANGLE_VEL
        
    def accelerate(self):
        self.thrust = True
        self.ship_thrust_sound.play()
        
    def no_pedal(self):
        self.thrust = False
        self.ship_thrust_sound.rewind()
    
    def stop_rotate(self):
        self.angle_vel = 0
        
    def move_to(position):
        print "plaat"
        
   #update a group of sprites
    def process_sprite_group(self, canvas, group):
        for g in group:
            g.draw(canvas)
        
        for g in set(group):
            if g.update():
                group.remove(g)
        
    def draw(self,canvas):
        if self.thrust:
            canvas.draw_image(self.image, (self.image_center[0]+self.image_size[0],self.image_center[1]), self.image_size, self.pos, self.image_size, self.angle)
        else:
            canvas.draw_image(self.image, self.image_center, self.image_size, self.pos, self.image_size, self.angle)
        self.process_sprite_group(canvas, self.missiles)
            
    def fire(self):
        direction = angle_to_vector(self.angle)
        self.missiles.add(Sprite([direction[0]*self.image_center[0]+self.pos[0], direction[1]*self.image_center[1]+self.pos[1]], [direction[0]*self.missile_mult,direction[1]*self.missile_mult], 0, 0, self.missile_image, self.missile_info, self.missile_sound))

    def update(self):
        if self.thrust:
            acceleration = angle_to_vector(self.angle)
            self.vel[0] += acceleration[0] * self.accel
            self.vel[1] += acceleration[1] * self.accel
        
        self.vel[0] *= self.friction
        self.vel[1] *= self.friction
        
        self.pos[0] = self.pos[0] + self.vel[0]
        self.pos[1] = self.pos[1] + self.vel[1]
        if self.pos[0] > WIDTH:
            self.pos[0] = 0
        elif self.pos[0] < 0:
            self.pos[0] = WIDTH
        
        if self.pos[1] > HEIGHT:
            self.pos[1] = 0
        elif self.pos[1] < 0:
            self.pos[1] = HEIGHT
        self.angle = self.angle + self.angle_vel
    
    
# Sprite class
class Sprite:
    def __init__(self, pos, vel, ang, ang_vel, image, info, sound = None):
        self.pos = [pos[0],pos[1]]
        self.vel = [vel[0],vel[1]]
        self.angle = ang
        self.angle_vel = ang_vel
        self.image = image
        self.image_center = info.get_center()
        self.image_size = info.get_size()
        self.radius = info.get_radius()
        self.lifespan = info.get_lifespan()
        self.animated = info.get_animated()
        self.age = 0
        if sound:
            sound.rewind()
            sound.play()
    
    def get_position(self):
        return self.pos
    
    def get_radius(self):
        return self.radius
   
    def draw(self, canvas):
        canvas.draw_image(self.image, self.image_center, self.image_size, self.pos, self.image_size, (self.angle/180.0)*math.pi)
    
    def update(self):
        self.angle += self.angle_vel
        if self.pos[0] + self.vel[0] + self.radius < 0:
            self.pos[0] = WIDTH + self.radius
        elif self.pos[0] + self.vel[0] - self.radius > WIDTH:
            self.pos[0] = 0 - self.radius
        else:
            self.pos[0] += self.vel[0]        
      
        if self.pos[1] + self.vel[1] - self.radius > HEIGHT:
            self.pos[1] = 0 - self.radius
        elif self.pos[1] + self.vel[1] + self.radius < 0:
            self.pos[1] = HEIGHT + self.radius
        else:    
            self.pos[1] += self.vel[1]
        self.age += 1
        return self.age > self.lifespan
            
    def collide(self, o):
        return dist(o.get_position(), self.get_position()) - o.get_radius() - self.get_radius() < 0

class Game(object):
    def __init__(self):
        self.init_info()
        # initialize frame
        self.frame = simplegui.create_frame("Asteroids", WIDTH, HEIGHT)
        self.time = 0.5
        
        self.score = 0
        self.lives = 3
        
        self.game_phase = START_SCREEN
        
        # initialize ship and two sprites
        self.my_ship = None
        self.rocks = set()
        self.missiles = set()
        
        # register handlers
        self.set_handlers()
        
        self.frame.start()
        self.timer.start()
        
    def game_start(self):
        self.game_phase = GAME_STARTED
        
        self.score = 0
        self.lives = 3
        self.time = 0.5
        
        #create the ship
        self.my_ship = Ship([WIDTH / 2, HEIGHT / 2], [0, 0], 2*math.pi, self.ship_image, self.ship_info)
        
    def game_over(self):
        self.game_phase = GAME_OVER
        
        #stop music and thruster sound
        self.my_ship.no_pedal()
        self.soundtrack.rewind()
        
        self.rocks = set()
        self.missiles = set()
        
        
        
           
    def draw(self, canvas):
        
        if self.game_phase == START_SCREEN:
            self.frame.set_canvas_background('Blue')
            canvas.draw_text('Press S to start the game', [50, 50], 32, 'White')
            self.soundtrack.rewind()
            self.soundtrack.play()
        
        else:
            # animiate background
            self.time += 1
            self.wtime = (self.time / 4) % WIDTH
            self.center = self.debris_info.get_center()
            self.size = self.debris_info.get_size()
            canvas.draw_image(self.nebula_image, self.nebula_info.get_center(), self.nebula_info.get_size(), [WIDTH / 2, HEIGHT / 2], [WIDTH, HEIGHT])
            canvas.draw_image(self.debris_image, self.center, self.size, (self.wtime - WIDTH / 2, HEIGHT / 2), (WIDTH, HEIGHT))
            canvas.draw_image(self.debris_image, self.center, self.size, (self.wtime + WIDTH / 2, HEIGHT / 2), (WIDTH, HEIGHT))
        
            # draw ship and sprites
            self.my_ship.draw(canvas)
            
            # update ship and sprites
            self.my_ship.update()
            
            if self.group_collide(self.my_ship, self.rocks):
                self.lives -= 0
                if self.lives <= 0:
                    self.game_over()
                    return
            self.process_sprite_group(canvas, self.rocks)
            self.score += self.group_group_collide(self.rocks, self.my_ship.missiles)
            
            if self.game_phase == GAME_OVER:
                canvas.draw_polygon([[100,100],[WIDTH-100,100],[WIDTH-100,HEIGHT-100],[100,HEIGHT-100]],4,'Red', 'White')
                canvas.draw_text('Game Over!', [120, 140], 32, 'Black')
                canvas.draw_text('You scored '+str(self.score)+' points!',[120, 166], 26, 'Black')
                canvas.draw_text('Click to restart the game', [120, 192], 26, 'Black')
            else:
                #set scores and lives
                canvas.draw_text('Score:', [20,34], 24, "Yellow")
                canvas.draw_text(str(self.score), [20, 58], 24, "Yellow")
                
                canvas.draw_text('Lives:', [WIDTH-100,34], 24, "Yellow")
                canvas.draw_text(str(self.lives), [WIDTH-100, 58], 24, "Yellow")
        
    #update a group of sprites
    def process_sprite_group(self, canvas, group):
        for g in group:
            g.draw(canvas)
        
        for g in group:
            g.update()
            
    def group_collide(self, o, group):
        for g in set(group):
            if g.collide(o):
                group.remove(g)
                return True
            
    def group_group_collide(self, o, group):
        removed = 0
        for g in set(group):
            if self.group_collide(g, o):
                removed += 1
                group.remove(g)
        return removed
            
    # timer handler that spawns a rock    
    def rock_spawner(self):
        print len(self.rocks)
        if len(self.rocks) < MAX_ROCKS and self.game_phase == GAME_STARTED:
            if self.time > 1800:
                speed = 1.9
            elif self.time > 1200:
                speed = 1.3
            else:
                speed = 0.8
            speed 		= speed * random.choice([-1, 1])
            spawn_pos 	= [random.randrange(0,WIDTH), random.randrange(0,HEIGHT)]
            rock_vel	= [random.random() * speed , random.random() * speed ]
            if dist(spawn_pos, self.my_ship.get_position()) > 2 * self.my_ship.get_radius():
                self.rocks.add(Sprite(spawn_pos, rock_vel, 0, random.randrange(-5, 5), self.asteroid_image, self.asteroid_info))
    
    def key_down_handler(self, key):
        if self.game_phase == GAME_STARTED:
            if simplegui.KEY_MAP['left'] == key:
                self.my_ship.rotate_left()
            elif simplegui.KEY_MAP['right'] == key:
                self.my_ship.rotate_right()
            elif simplegui.KEY_MAP['up'] == key:
                self.my_ship.accelerate()
            elif simplegui.KEY_MAP['space'] == key:
                self.my_ship.fire()
        elif self.game_phase == START_SCREEN:
            if simplegui.KEY_MAP['s'] == key:
                self.game_start()
    
    def key_up_handler(self, key):
        if self.game_phase == GAME_STARTED:
            if simplegui.KEY_MAP['left'] == key or simplegui.KEY_MAP['right'] == key:
                self.my_ship.stop_rotate()
            elif simplegui.KEY_MAP['up'] == key:
                self.my_ship.no_pedal()
                
    def mouse_handler(self, pos):
        if self.game_phase == GAME_OVER:
            self.game_start()
        elif self.game_phase == GAME_STARTED:
            self.my_ship.move_to(pos)
    
    def set_handlers(self):
        self.frame.set_draw_handler(self.draw)
        self.frame.set_keydown_handler(self.key_down_handler)
        self.frame.set_keyup_handler(self.key_up_handler)
        self.timer = simplegui.create_timer(1000.0, self.rock_spawner)
        self.frame.set_mouseclick_handler(self.mouse_handler)
        
    
    def init_info(self):
        self.debris_info = ImageInfo([320, 240], [640, 480])
        self.debris_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/debris2_blue.png")
        
        self.nebula_info = ImageInfo([400, 300], [800, 600])
        self.nebula_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/nebula_blue.f2014.png")
        
        # splash image
        self.splash_info = ImageInfo([200, 150], [400, 300])
        self.splash_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/splash.png")
        
        # ship image
        self.ship_info = ImageInfo([45, 45], [90, 90], 35)
        self.ship_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/double_ship.png")
                
        # asteroid images - asteroid_blue.png, asteroid_brown.png, asteroid_blend.png
        self.asteroid_info = ImageInfo([45, 45], [90, 90], 40)
        self.asteroid_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/asteroid_blue.png")
        
        # animated explosion - explosion_orange.png, explosion_blue.png, explosion_blue2.png, explosion_alpha.png
        self.explosion_info = ImageInfo([64, 64], [128, 128], 17, 24, True)
        self.explosion_image = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/lathrop/explosion_alpha.png")
        
        # sound assets purchased from sounddogs.com, please do not redistribute
        self.soundtrack = simplegui.load_sound("http://commondatastorage.googleapis.com/codeskulptor-assets/sounddogs/soundtrack.mp3")

if __name__ == "__main__":
    Game()