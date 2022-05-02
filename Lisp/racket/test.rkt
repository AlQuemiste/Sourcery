;; racket -l racket/base -f test.scm

(require racket/system)
(require racket/port)

(displayln "Hello world!")
(define cmd (format "ls ~a" "-lh"))
(define cmd "date")
(define (runcmd cmd)
  (with-output-to-string
    (lambda () (system cmd)))
  )

(define res (runcmd cmd))
(display (format "output=\"~n~a\"~n" res))
(displayln "----")


(define mtch (regexp-match #rx"-[0-9]*" "a-12--345b"))
(println mtch)

(define mtch (regexp-match* #rx"(-[0-9]*)" "a-12--345b"))
(println mtch)
