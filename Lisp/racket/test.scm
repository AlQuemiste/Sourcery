;; see
;; <https://www.scheme.com/tspl4>
;; <http://www.r6rs.org>
;; <https://groups.csail.mit.edu/mac/ftpdir/scheme-7.5>
;; https://scheme.com/csug8/
;; https://book.crazydan.org/chez-scheme-version-9-user-guide/en/index.html
;; execute:
;; chezscheme --script <script-file>

;(import (rnrs base (6)))
(import (chezscheme))

(display "Hello world! ")
(display (char-upcase #\c))
(display (command-line))
(newline)
(define cmd (format "ls ~a" "-lh"))
(define cmd "date")
(define (runcmd cmd)
  (with-output-to-string
    (lambda () (system cmd)))
  )

(define res (runcmd cmd))
(display (format "output=\"~n~a\"~n" res))
(display "----")
(newline)


(define res1
  (with-output-to-string
    (lambda ()
      (system "ls")
      (display "Once upon a time ...\n."))
    )
  )

(display "res1 =\n")
(display res1)
(newline)

;; (define mtch (string-match #rx"-[0-9]*" "a-12--345b"))
;; (println mtch)

;; (define mtch (regexp-match* #rx"(-[0-9]*)" "a-12--345b"))
;; (println mtch)
