# docker build -t doc-builder .
# docker run --rm -v $(pwd):/docs doc-builder todo.md output.pdf

FROM ubuntu:22.04

# Install required packages
RUN apt-get update
RUN apt-get install -y gpp
RUN apt-get install -y  pandoc
RUN apt-get install -y  texlive-latex-base
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y  texlive-latex-extra
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y  texlive-fonts-recommended
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y texlive
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y texlive-fonts-extra
# Set up working directory
WORKDIR /docs

COPY eisvogel.tex /docs/eisvogel.tex

ADD https://git.umaneti.net/latex-awesomebox/plain/awesomebox.sty /usr/share/texlive/texmf-dist/tex/generic/awesomebox.sty

RUN texhash


# Create entrypoint script
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]

