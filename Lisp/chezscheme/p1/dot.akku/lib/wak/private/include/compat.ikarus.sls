#!r6rs
;;; compat.ikarus.sls --- include compatibility for Ikarus

;; Copyright (C) 2009, 2010 Andreas Rottmann <a.rottmann@gmx.at>

;; This program is free software, you can redistribute it and/or
;; modify it under the terms of the MIT/X11 license.

;; You should have received a copy of the MIT/X11 license along with
;; this program. If not, see
;; <http://www.opensource.org/licenses/mit-license.php>.

;;; Commentary:

;;; Code:

(library (wak private include compat)
  (export stale-when
          annotation?
          annotation-expression
          read-annotated
          file-mtime
          merge-path
          (rename (library-path library-search-paths)))
  (import (rnrs base)
          (wak private include utils)
          (only (ikarus)
                stale-when
                annotation?
                read-annotated
                annotation-expression
                library-path
                file-mtime))

(define (merge-path path origin)
  (string-append origin "/" (string-join path "/")))

)
