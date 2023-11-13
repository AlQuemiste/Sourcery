(define (capture-standard-output command)
  (let-values ([(in out err pid) (open-process-ports command 'block
                                   (make-transcoder (utf-8-codec)))])
    (get-string-all out)))


(define out (capture-standard-output "lsx -a"))  ;; => ".\n..\nbar\nfoo\n"
(display "\"\"\"")
(newline)
(display out)
(display "\"\"\"")
(newline)

