;; A simple Scheme code which executes a given shell command with some parameters and captures the stdout and stderr output in two separate strings.
;; 1) uses Chez-Scheme standard (essentially, within R6RS standard plus Chez additional built-in modules).
;; 2) runs the shell command as a blocking statement and waits until the end of the command executation.
;; 3) is platform independent and usable on Linux, MacOS and Windows.
;; 4) finally displays the captured stdout and stderr strings.

;; Import the necessary R6RS library for I/O ports, including get-string-all,
;; make-transcoder, utf-8-codec, and related procedures.
(import (rnrs io ports))

;; Helper procedure to join a list of strings with a given separator.
;; This is not part of R6RS, so we define it recursively for simplicity.
;; Example: (string-join '("hello" "world") " ") => "hello world"
(define string-join
  (lambda (strs separator)
    (if (null? strs)
        ""
        (let ((first (car strs))
              (rest (cdr strs)))
          (if (null? rest)
              first
              (string-append first separator (string-join rest separator)))))))

;; Main procedure to execute a shell command with parameters.
;; - command: string, the base command (e.g., "echo").
;; - params: zero or more strings, the parameters (e.g., "hello").
;;   These are collected into a list via varargs.
;; Constructs the full command string by joining with spaces,
;; adding a space after the command only if parameters are present.
;; Uses Chez Scheme's open-process-ports to create a subprocess.
;; Captures stdout and stderr in separate textual ports using a UTF-8 transcoder.
;; Closes stdin immediately since no input is provided.
;; Reads all output using get-string-all, which blocks until EOF (process exit).
;; This ensures the call is blocking overall.
;; Returns two values: stdout string and stderr string.
;; Platform-independent: open-process-ports handles shell invocation appropriately
;; (e.g., /bin/sh on Unix-like, cmd.exe on Windows).
(define execute-shell-command
  (lambda (command . params)
    ;; Join parameters with spaces.
    (let ((joined-params (string-join params " ")))
      ;; Build the full command string, e.g., "echo hello".
      ;; Add space after command only if params are present.
      (let ((full-command (if (null? params)
                              command
                              (string-append command " " joined-params))))
        ;; Open process ports: to-stdin (output), from-stdout (input), from-stderr (input), pid.
        ;; Specify 'block buffer mode and UTF-8 transcoder for textual ports.
        ;; Note: Specify (utf-8-codec) explicitly to avoid compiler warning.
        (let-values (((to-stdin from-stdout from-stderr pid)
                      (open-process-ports full-command 'block (make-transcoder (utf-8-codec)))))
          ;; No input to subprocess, so close stdin port.
          (close-port to-stdin)
          ;; Read all output from stdout until EOF (blocks until process exits).
          (let ((stdout-str (get-string-all from-stdout)))
            ;; Read all output from stderr until EOF (blocks if necessary).
            (let ((stderr-str (get-string-all from-stderr)))
              ;; Clean up ports.
              (close-port from-stdout)
              (close-port from-stderr)
              ;; Return the captured strings.
              (values stdout-str stderr-str))))))))

;; Test the implementation.
;; Execute "echo hello", which should output "hello\n" to stdout and empty to stderr.
;; Call with varargs: command followed by parameter strings directly.
;; Display the results for verification.
;; Expected: STDOUT: "hello\n" (exact output may vary slightly by platform, e.g., CRLF on Windows)
;;           STDERR: "" (empty string)
(let-values (((stdout stderr) (execute-shell-command "echo" "hello")))
  (display "STDOUT:\n")
  (display stdout)
  (newline)
  (display "STDERR:\n")
  (display stderr)
  (newline))
