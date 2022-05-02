#lang racket

(define-syntax-rule (or x y)
  (if x x y))

(define-syntax-rule (or1 x y)
  (let ([z x])
  (if z z y)))


;; (println "Checks:")
;; (println "1:")
;; (or1 3 (/ 2 0))

;; (println "2:")
;; (or1 (begin (display 3) 3) (/ 2 0))


(define (assert-bad expr)
  (when (not expr)
    (error 'assert "assertion fail: ~s" expr))
  )

(define-syntax-rule (assert expr)
  (when (not expr)
    (error 'assert "assertion failed: ~s" (quote expr))))

(define-syntax-rule (print-val expr)
  (printf "~a ==> ~a" (quote expr) expr))

(define X 42)

(define-syntax-rule (my-or e1 e2)
  (let ([x e1])
    (if x (begin (print "CASE I") x) (begin (printf "CASE 2, X = ~s" X) e2)))
  )

;; https://www.greghendershott.com/fear-of-macros

(define-syntax foo
  (lambda (stx1)
    (syntax "I am foo")))

(define-syntax (say-hi stx)
  (syntax (displayln "hi"))
  )

(define-syntax (show-me stx)
  (print stx)
  #'(void))

(define-syntax (my-get stx)
  (let* ([dt (syntax->datum stx)]
         [st (cadr dt)]
         [fld (caddr dt)]
         [var (cadddr dt)]
         [st-fld (string->symbol (format "~a-~a" st fld))])

    (begin
      (printf ">> st = '~a', fld = '~a', var = '~a', st-fld = '~a'~n"
              st var fld st-fld)
      (datum->syntax stx `((unquote st-fld) ,var))
      )
    )
  )

(struct bar (x y))
(define b0 (bar 1 2))
(my-get bar y b0)


(require (for-syntax racket/match))
(define-syntax (my-get-v2 stx)
  (match (syntax->datum stx)
    [(list _ st fld var)
     (let ([st-fld (string->symbol (format "~a-~a" st fld))])
       (datum->syntax stx `(,st-fld ,var)))
     ]
    )
  )
