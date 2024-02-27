SHELL=/bin/bash -e -o pipefail
PWD = $(shell pwd)

# constants
GOLANGCI_VERSION = 1.56.2
DOCKER_REPO = server
DOCKER_TAG = latest

# CLIENT
CC=clang
CFLAGS=-g -Wall
OBJS=client/ring_buffer.o 
BIN=client/buffer_test
RM=rm -f

#MUTUAL

all: git-hooks tidy ${BIN} ## Initializes all tools

clean: clean-client clean-server ## Cleans up everything
# CLIENT
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): client/ring_buffer.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	
clean-client:
	$(RM) -r $(OBJS) $(BIN) *.dSYM


# SERVER

out:
	@mkdir -p out

git-hooks:
	@cd server && git config --local core.hooksPath .githooks/

download: ## Downloads the dependencies
	@cd server && go mod download

tidy: ## Cleans up go.mod and go.sum
	@cd server && go mod tidy

fmt: ## Formats all code with go fmt
	@cd server && go fmt ./...

run: fmt ## Run the app
	@cd server && go run ./cmd/server/main.go

test-build: ## Tests whether the code compiles
	@cd server && go build -o /dev/null ./...

build: out/bin ## Builds all binaries

GO_BUILD = mkdir -pv "$(@)" && cd server && go build -ldflags="-w -s" -o "../$(@)" ./...
.PHONY: out/bin
out/bin:
	$(GO_BUILD)

GOLANGCI_LINT = bin/golangci-lint-$(GOLANGCI_VERSION)
$(GOLANGCI_LINT):
	curl -sSfL https://raw.githubusercontent.com/golangci/golangci-lint/master/install.sh | bash -s -- -b bin v$(GOLANGCI_VERSION)
	@mv bin/golangci-lint "$(@)"

lint: fmt $(GOLANGCI_LINT) download ## Lints all code with golangci-lint
	@cd server && $(GOLANGCI_LINT) run

lint-reports: out/lint.xml

.PHONY: out/lint.xml
out/lint.xml: $(GOLANGCI_LINT) out download
	@cd server && $(GOLANGCI_LINT) run ./... --out-format checkstyle | tee "$(@)"

test: ## Runs all tests
	@cd server && go test $(ARGS) ./...

coverage: out/report.json ## Displays coverage per func on cli
	cd server && go tool cover -func=../out/cover.out

html-coverage: out/report.json ## Displays the coverage results in the browser
	cd server && go tool cover -html=../out/cover.out

test-reports: out/report.json

.PHONY: out/report.json
out/report.json: out
	@cd server && go test -count 1 ./... -coverprofile=../out/cover.out --json | tee "../$(@)"

clean-server: ## Cleans up everything
	@cd server && rm -rf ../bin ../out

docker: ## Builds docker image
	cd server && docker buildx build -t $(DOCKER_REPO):$(DOCKER_TAG) .

ci: lint-reports test-reports ## Executes lint and test and generates reports

help: ## Shows the help
	@echo 'Usage: make <OPTIONS> ... <TARGETS>'
	@echo ''
	@echo 'Available targets are:'
	@echo ''
	@grep -E '^[ a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
        awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
	@echo ''
