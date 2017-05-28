#! /usr/bin/python
# -*- encoding: utf-8 -*-

"""
[1] Working without settings with django template engine 
http://stackoverflow.com/questions/98135/how-do-i-use-django-templates-without-the-rest-of-django
[2] Django Template 
https://docs.djangoproject.com/en/1.11/ref/templates/api/
[3] Django Engine Source
https://docs.djangoproject.com/en/1.11/_modules/django/template/engine/#Engine.get_template
[4] Autoscape django template
http://stackoverflow.com/questions/11306669/django-template-escaping
"""

from django.template import Context, Template
from django.template.engine import Engine
import os

# Templates directories
BASE_DIR = os.path.dirname(os.path.realpath(__file__))
TEMPLATE_DIRS = ( 
    os.path.join(BASE_DIR,  'Templates', 'Sequential'), 
)

# Template context structure

# initialize = """
#     int i = 0; int j = 0;
#     for (i=0; i<Dy; ++i){
#         for (j=0; j<Dx; ++j) matrix[i][j] = i+j;
#     }
# """

initialize = """
    double pi = 3.14159265358979323846;
    clock_t t;
    t=clock();
    double tc;
    tc = (((float)t)/CLOCKS_PER_SEC);
    long double X = fmod ((pi*t)+(pi/tc),(pi/tc));/*Seed*/
    double M = 2147483648,n;
    int a = 1103515245,c=12345,i;
    for(i=1;i<5;i++){
        X = fmod ((a*X+c),M);// Linear congruence
        n = (X/(M-1));
    }  
    return (n < 1) ? 1 : 0;
"""

# struct Neighborhood
# {
    # left_up;        up;         right_up;
    # left;           center;     right; 
    # left_down;      down;       right_down;
# };

# Transition function might have a call to a neighboohood function
# and return a value equal to states.type (a new state)
transition = r"""
    struct Neighborhood nbhd = neighborhoodOf(matrix,xcoor,ycoor);

    int sum = nbhd.left_up + nbhd.up + nbhd.right_up + nbhd.left + nbhd.right + nbhd.left_down + nbhd.down + nbhd.right_down;
    int site = nbhd.center;
    int life = ( site == 1 ) ? ( sum == 2 || sum == 3 ) ? 1:0 : ( sum == 3 ) ? 1:0;

    return life;
"""

ca = {
    'libraries':['math.h','time.h'],
    'constants':[('PI','3.14159265358979323846')],
    'lattice': { 
        'size': {
                'x': '10',
                'y': '10'
            }, 
        'states': { 
                'type': 'bool', 
                'initialize': initialize, 
                'transition': transition
            } 
        },
    'run': {
        'generations': '12',
        'up_to_condition': '',
        'debug': '1'
    }
}

# Django Templates Engine
engine = Engine(dirs=TEMPLATE_DIRS)
template = engine.get_template('MatrixLifeCA.c')
raw_code = template.render(Context(ca))

with open('MatrixLifeCA.parsed.c','w') as cfile:
    cfile.write(raw_code)



