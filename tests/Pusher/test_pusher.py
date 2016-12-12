#!/usr/bin/env python
#!coding: utf-8



import odepush


import sys


def main():

    if len(sys.argv) ==2:
        path = sys.argv[1]
    else:
        path = './'

    odepush.main(path)




if __name__ == "__main__":
    main()
