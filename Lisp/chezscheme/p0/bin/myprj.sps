#!/usr/bin/env scheme-script
;; -*- mode: scheme; coding: utf-8 -*- !#
;; Copyright (c) 2022 Guy Q. Schemer
;; SPDX-License-Identifier: MIT
#!r6rs

(import (rnrs (6)) (myprj) (wak irregex))

(display (hello "World"))
(newline)

(define match0
  (irregex-match '(w/nocase "foobar") "abcFOOBARdef"))
(display match0)
(newline)
