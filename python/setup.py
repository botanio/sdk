from setuptools import setup

setup(name='botanio',
      version='1.0.0',
      description='Botan SDK: Advanced analytics for your Telegram bot',
      url='https://github.com/botanio/sdk',
      author='Botan.io team',
      author_email='team@botan.io',
      license='MIT',
      packages=['botanio'],
      install_requires=[
          'requests',
      ],
      zip_safe=False)
