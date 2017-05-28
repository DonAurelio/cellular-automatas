#! /usr/bin/python
# -*- encoding: utf-8 -*-

import string 

class Test(object):
	"""
		Test class

		hello.
	"""
	def __init__(self):
		"""
			Hello world.
		"""
		self.a = "Z"
		self.b = {'w':1,'k':2}

obj = Test()
template = string.Template("Hola soy ${aa} y ${b}")
template_format = "Hola soy {a[name][first]} y {b}".format(a={'name':{'first':'aurelio','last':'vivas'},'age':'16'},b={'name':'b1','age':'20'})
print template.substitute({'aa':{'1':'L','2':'Z'},'b':'B'})
print template_format
# print obj.__dict__
# print obj.__doc__

"""
[1] Working without settings with django template engine 
http://stackoverflow.com/questions/98135/how-do-i-use-django-templates-without-the-rest-of-django
[2] Django Template 
https://docs.djangoproject.com/en/1.11/ref/templates/api/
"""
# -*- encoding: utf-8 -*-
from django.template import Context, Template
from django.template.engine import Engine
from django.conf import settings
import os

BASE_DIR = os.path.dirname(os.path.realpath(__file__))
TEMPLATE_DIRS = ( 
	os.path.join(BASE_DIR,  'Templates', 'Sequential'), 
)

# settings.configure(INSTALLED_APPS=INSTALLED_APPS,TEMPLATES=TEMPLATES)
# settings.configure(TEMPLATE_DIRS=(".",))
# settings.configure(TEMPLATE_DIRS=TEMPLATE_DIRS)

print BASE_DIR
print TEMPLATE_DIRS

MLCATemplateFile = 'MatrixLifeCA.c'
MLCATemplate = None

lattice = {
	'lattice': { 
		'size': {
				'x':'10',
				'y':'10'
			}, 
		'states': { 
				'type':'int', 
				'initialize':'NULL', 
				'transition':'NULL' 
			} 
		}
}

# The following lines needs the settings configurations
# with open(MLCATemplateFile,'r') as template:
# 	MLCATemplate = Template(template.read(),engine=Engine())
# print MLCATemplate.render(Context(lattice))

# from django.template.loader import get_template
# template = get_template(MLCATemplateFile)
# print template.render(Context(lattice))

# Engine works without setting config
# engine = Engine(dirs=TEMPLATE_DIRS)
# template = engine.get_template(MLCATemplateFile)
# print template.render(Context(lattice))

