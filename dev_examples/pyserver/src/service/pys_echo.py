#!/bin/env python
# -*- coding: utf-8 -*-
    

def Entry(req,context):
    return {'cmd':'_echorsp','data':req['data']}
