use actix_web::{web, App, HttpServer, Responder, HttpResponse};
use regex::Regex;
use std::process::Command;

async fn index() -> impl Responder {
    HttpResponse::Ok().content_type("text/html").body(include_str!("../templates/index.html"))
}

async fn get_results() -> impl Responder {
    let output = if let Ok(output) = Command::new("./1").output() {
        output
    } else {
        return HttpResponse::InternalServerError().finish();
    };

    let output_str = String::from_utf8_lossy(&output.stdout);

    let pattern = Regex::new(r"Average (.*?) Time: (.*?) seconds.").unwrap();
    let mut results = serde_json::Map::new();

    for cap in pattern.captures_iter(&output_str) {
        if let (Some(name), Some(time)) = (cap.get(1), cap.get(2)) {
            let time_value: f64 = time.as_str().parse().unwrap_or(0.0);
            results.insert(name.as_str().to_string(), serde_json::json!(time_value));
        }
    }

    HttpResponse::Ok().json(results)
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {
        App::new()
            .route("/", web::get().to(index))
            .route("/get_results", web::get().to(get_results))
    })
    .bind("127.0.0.1:8080")?
    .run()
    .await
}

