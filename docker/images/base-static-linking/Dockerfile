ARG NODE_VERSION=12.16.3
FROM node:${NODE_VERSION} as builder

RUN  set -x \
  && apt update \
  && apt install -y gyp \
      python3 \
      make \
      g++ \
      libunwind-dev

ADD . /var/app

WORKDIR /var/app

RUN npm install --unsafe-perm

FROM node:${NODE_VERSION}

COPY --from=builder /var/app/ /var/app/
COPY ./docker/scripts/entrypoint.sh /bin/entrypoint

WORKDIR /var/app

RUN chmod +x /bin/entrypoint

ENTRYPOINT [ "/bin/entrypoint" ]
CMD ["node", "tests/test.js"]
