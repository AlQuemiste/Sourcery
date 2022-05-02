; see <https://github.com/cisco/ChezScheme/issues/604>

(define (println str) (display str) (newline))

(define (fmt/1 cmd . args)
  (format "~s~{ ~s~}" cmd args))


(println (fmt/1 "ls" "-t 1" "-x 2" "-y 3"))

(define-values (from-stdout to-stdin pid)
  (apply values 
    (process (fmt/1 "ls" "-l" "-a"))
    )
  )

(close-output-port to-stdin)
(define out (get-string-all from-stdout))

(println "====")
(display "output =\n")
(display out)
(println "====")

#|
(define (system* cmd . args)
  (apply (lambda (from-stdout to-stdin pid)
  (close-output-port to-stdin)
  (display (get-string-all from-stdout))
  pid)
  (process (format "~s~{ ~s~}" cmd args))))
    |#
    