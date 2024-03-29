#!r6rs
;;; mosh.mzscheme.sls --- include compatibility for Mosh

;; Copyright (C) 2010 Andreas Rottmann <a.rottmann@gmx.at>

;; This program is free software, you can redistribute it and/or
;; modify it under the terms of the MIT/X11 license.

;; You should have received a copy of the MIT/X11 license along with
;; this program. If not, see
;; <http://www.opensource.org/licenses/mit-license.php>.

;;; Commentary:

;;; Code:

(library (wak private include compat)
  (export stale-when
          read-annotated
          annotation?
          annotation-expression
          file-mtime
          merge-path
          library-search-paths)
  (import (rnrs base)
          (rnrs io simple)
          (wak private include utils)
          (prefix (only (mosh)
                        library-path)
                  mosh:)
          (mosh file))

(define-syntax stale-when
  (syntax-rules ()
    ((_ conditition body ...)
     (begin body ...))))

(define (read-annotated port)
  (read port))

(define (annotation? thing)
  #f)

(define (annotation-expression thing)
  thing)

(define (merge-path path origin)
  (string-append origin "/" (string-join path "/")))

(define (file-mtime filename)
  (file-stat-mtime filename))

(define (library-search-paths)
  (mosh:library-path))

)
