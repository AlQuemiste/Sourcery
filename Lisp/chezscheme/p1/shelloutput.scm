(import (rnrs)) ; import the R6RS standard library

(define (run-shell-command command)
  (call-with-input-string
    (open-input-pipe command)
    get-string-all))

(define result (run-shell-command "ls -l"))

(display result)
